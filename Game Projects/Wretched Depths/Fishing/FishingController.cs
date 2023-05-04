using UnityEngine;
using UnityEngine.InputSystem;

public class FishingController : MonoBehaviour
{
    public enum FishingState
    {
        None,
        Casting,
        Idle,
        Reeling

    }

    [Header("References")]
    public CamTargetControl ctc;                //camera target control for fishing camera
    public Rigidbody rodTip;                    //rigidbody of the rod tip
    public RopeControllerVisual rodRope;        //rope that starts at the rod tip
    public LureController lureController;             //lure controller
    public GameObject bobberGroupPrefab;        //prefab for the bobber group
    public LineRenderer fishingLine;

    [Header("Cast Tracking Settings")]
    public float castSpeed = 25f;               //speed of the cast
    public float reelSpeed = 15;                //speed of reeling in
    public float reelAcceleration = 30f;        //acceleration when realing in

    [Header("Fishing Settings")]
    public int fishingSegmentCount = 10;        //number of segments when fishing
    public int idleSegmentCount = 2;            //number of segments when fishing
    public float idleHangDistance = 0.25f;      //distance that the lure hangs when idling
    public float lureStopRadius = 0.75f;        //distance where the lure stop
    public float fishCatchDistance = 20f;

    //Variables
    public Vector3 castPosition { get; private set; } = Vector3.zero;   //position we are casting towards
    private FishingState state = FishingState.None;                     //current state in the fishing cycle
    private BobberGroupController bobberGroup;                          //current Bobber in level
    public float tensionAccum = 0;                                      //accumulation float for rope tension
    private bool bobberMarkedDestroy = false;
    private int catchDepth = 0;                                             // Record what depth a fish is caught
    public float tensionMax = 1100f;
    private bool lure_locked = false;
    private bool canReel = true;

    private GameManager gm;
    private PlayerInput input;
    private RodScriptableObject rod;


    public void OnEndingCutscene() 
    { 
        canReel = false;
        //todo prevent reel in fishing controller (r and left click)
        InputAction.CallbackContext c = new InputAction.CallbackContext();
        EndReel(c);
        // prevent fish from chasing lure 
        lureController.isAttractive = false;
        HoldLure(c);
    }

    //HELPER BOOLS
    public bool isReeling { get { return state == FishingState.Reeling; } }
    public bool isIdle { get { return state == FishingState.Idle; } }
    public bool isCasting { get { return state == FishingState.Casting; } }
    public bool isFishing { get { return state != FishingState.None; } } //if state is NOT none

    private bool HOLD_TOGGLE_SETTING = false; // NOTE: get from settings. Uncertain where to include it so leaving settings menu alone ~X

    private bool toggle_lure_locked = false;


    //SFX Variables
    private bool has_struggle_played = false;
    private AudioSource tempReelSFX;

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.green;
        Gizmos.DrawWireSphere(transform.position, fishCatchDistance);
    }

    //Reference to fish                      
    public FishBehaviour attachedFish { get; private set; }     //current fish

    void Awake() 
    { 
        SetState(FishingState.None);
        reelSFXBandaid();
        gm = GameManager.Instance;
        input = ControlScheme.Instance.input;
        rod = GameManager.Instance.inventoryCont.GetCurrentRod();
    }
    
    void Update()
    {
        HoldLureWithFish();
        //delete bobber if marked for deletion
        if (bobberMarkedDestroy)
        {
            Destroy(bobberGroup.gameObject);        //destroy the bobber
            bobberGroup = null;                     //reference is now null
            bobberMarkedDestroy = false;
        }

        //TODO reel SFX HERE
        //ambientSFX.volumeMultiplier = AudioListener.volumeMultiplier;

        //----LOOK----
        ctc.UpdateMouseMovement(input.actions["F_Look"].ReadValue<Vector2>());

        //----RETRACT----
        if (canReel && input.actions["F_Retract"].triggered)  //if we have triggered instant retract
        {
            if (!attachedFish) Retract();
            return;
        }
        if (state == FishingState.Reeling && GetLureCloseTo(rodTip.transform.position)) //if we are reeling and close to rod
        {
            Retract();
            return;
        }

        //-----WATER LEVEL------
        //if below water level
        bool isBelowWater = lureController.transform.position.y < gm.GetWaterLevel();
        if (isBelowWater)
        {
            if (state != FishingState.None && bobberGroup == null) { AddBobber(); }        //if not NONE try to add a bobber
            if (state == FishingState.Casting)                            // if we are casting
            {
                Debug.Log("I don't think anything executes here. Let X know if this is printed.");
                lureController.rb.velocity = Vector3.zero;        //stop velocity
                SetState(FishingState.Idle);                            //end the cast and set state to idle
            }
        }
        //if above water level
        else
        {
            RemoveBobber();                             //try to remove a bobber
            if (attachedFish != null && isFishing)
            {
                if (state == FishingState.Reeling)             // Check player is actively reeling
                {
                    float dist = Vector3.Distance(gm.playerCont.transform.position, lureController.transform.position);
                    if (dist < fishCatchDistance) Retract(true);     // Check fish is close enough to player
                    //else RetractLineBroke();
                }
                else Retract(false, true);
            }    //if there is a fish, return with it
        }

        //---------STATE----------

        switch (state)
        {
            //NOT FISING
            case FishingState.None:
                break;

            //CASTING OUT
            case FishingState.Casting:
                if (GetLureCloseTo(castPosition))   //we have reached the cast position goal
                {
                    SetState(FishingState.Idle);                            //we have reached the cast goal, no longer casting
                    lureController.rb.velocity = Vector3.zero;        //stop lure movement
                }
                else                                //else we havent reached the goal
                {
                    Vector3 goalVel = castSpeed * (castPosition - lureController.transform.position).normalized;
                    lureController.rb.velocity = goalVel;             //velocity is towards cast point

                }
                break;

            //LURE IDLING
            case FishingState.Idle:
                //do nothing
                break;

            //REELING IN
            case FishingState.Reeling:
                Vector3 lurePos = lureController.transform.position;
                Vector3 lurePosXY = Util.V3ZeroY(lurePos);
                Vector3 rodTipPos = rodTip.transform.position;
                Vector3 rodTipPosXY = Util.V3ZeroY(rodTipPos);


                //release the lure if locked and there is a fish
                if (attachedFish && lure_locked)
                {
                    ReleaseLure(new InputAction.CallbackContext());
                }

                //DETERMINE GOAL DIRECTION
                //if the bobber exists
                Vector3 goalDirection = Vector3.zero;
                if (bobberGroup)
                {
                    Vector3 bobberPos = bobberGroup.bobber.transform.position;
                    Vector3 bobberPosXY = Util.V3ZeroY(bobberPos);

                    //if the lure should be "locked"
                    float lockDistance = 0.75f;
                    float reelInDistance = 10;
                    float overrideDistance = .1f;
                    float b = 1;
                    float a = -1;
                    //if lure is close enough ignore everything and just pull it in
                    if (Vector3.Distance(bobberPosXY, lurePosXY) > overrideDistance)
                    {
                        //if the lure is locked, then increace y velocity when under the reelInDistance threshold
                        if (Mathf.Abs(bobberPos.y - lurePos.y) < lockDistance)
                        {
                            a = Mathf.Max(0, reelInDistance - Vector3.Distance(lurePosXY, rodTipPosXY));
                            b = 4 * Mathf.InverseLerp(reelInDistance / 4, reelInDistance * 3 / 4, a);

                        }
                        //otherwise if the lure is not locked decrease the y velocity by distance tot he bobber
                        else
                        {
                            a = Mathf.Max(0, Vector3.Distance(lurePos, bobberPos) - lockDistance); //always greater than zero in this loop
                            b = Mathf.Clamp(a / 4, 0, 1);
                        }
                        //Debug.Log(a + "    " + b);
                    }
                    //goalDirection = Util.V3ReplaceY((rodtipPos - bobberPos).normalized, 0);
                    goalDirection = (bobberGroup.bobber.transform.position - lurePos).normalized;
                    goalDirection = Util.V3MultiplyY(goalDirection, b).normalized;
                }
                //otherwise, bobber doesnt exist then
                else
                {
                    goalDirection = (rodTipPos - lurePos).normalized;
                }

                //DETERMINE GOAL VELOCITY
                float fishAttachedReelSpeedModifier = 1;
                Vector3 goalVelocity = reelSpeed * (attachedFish ? 1 : fishAttachedReelSpeedModifier) * goalDirection;    //set velocity to pull the lure in
                lureController.rb.velocity = Vector3.MoveTowards(lureController.rb.velocity, goalVelocity, Time.deltaTime * reelAcceleration);                   

                break;

            //UNKNOWN STATE
            default:
                break;


        }
        DoStruggleSFX();

        DoCameraShake();

        rod = GameManager.Instance.inventoryCont.GetCurrentRod();       //Gets the current rod
        DoTensionUpdate();

        ReapplyLureHold();

        //hide the lure mesh if in a fish's mouth
        lureController.SetLureVisible((attachedFish == null));
    }

    public void DoTensionUpdate()
    {
        if (!attachedFish) return;

        float amt = 0f;

        //FISH NOT FIGHTING
        if (!attachedFish.isFighting)
        {
            amt = -1 * Time.deltaTime * 200f;
        }
        //FISH FIGHTING

        //REELING
        else if (state == FishingState.Reeling)
        {
            amt = Time.deltaTime * (rod.strength * gm.GetFish(attachedFish.id).tensionPerSecond);
        }

        //IDLE
        else if(state == FishingState.Idle)
        {
            //LURE LOCKED
            if (lure_locked)
            {
                //still add a quarter of tension to discourage holding the fish for too long
                amt = Time.deltaTime * gm.GetFish(attachedFish.id).tensionPerSecond / 4;
            }
            //LURE UNLOCKED
            else if (tensionAccum > 0f)
            {
                amt = -1 * Time.deltaTime * 100f;
            }
        }
        
        tensionAccum = Mathf.Max(tensionAccum + amt, 0);

        //Debug.Log(amt.ToString("0.0000") + "   " + tensionAccum.ToString("0.00"));

        //check for line break
        if (tensionAccum >= tensionMax)
        {
            Retract(false, true);
        }
    }

    public void DoCameraShake()
    {
        if(!attachedFish) return;
        if((state == FishingState.Reeling || lure_locked))
        {
            if (attachedFish.isFighting)
            {
                float shake = tensionAccum / tensionMax / (lure_locked ? 4 : 1);
                gm.SetCameraShake(shake);
            }
            else
            {
                gm.SetCameraShake(0);
            }
        }
        if((state == FishingState.Idle)) 
        {
            gm.SetCameraShake(0);
        }
    }

    //-----------------------BOBBER --------------------------------------------------

    public void AddBobber()
    {
        AudioManager.Instance.Play("RodBobber");
        if (bobberGroup == null)    //if there is no bobber rig object
        {
            bobberGroup = Instantiate(bobberGroupPrefab, transform).GetComponent<BobberGroupController>();    //create new bobber
            bobberGroup.transform.position = Util.V3ReplaceY(lureController.transform.position, gm.GetWaterLevel() + 0.05f);     //place the bobber just above water level
            bobberGroup.Init(lureController.gameObject, rodTip.transform);    //initialize bobber (this sets up the movement transforms and sets the rope connection for the bobber rope)

            lureController.AddSpringJoint(bobberGroup.rb);        //limit the depth of the lure by adding a joint to the bobber ((TODO)) make this directly vertical (to another invisible RB that moves with the lure?)

            rodRope.SetRopeEnd(bobberGroup.bobber);     //set the end of the rope to the bobber
        }
    }

    private void MarkBobberDestroy() { bobberMarkedDestroy = true; }
    public void RemoveBobber()
    {
        if (bobberGroup != null)    //if there is a bobber rig object
        {          
            if(state!=0) lureController.RemoveSpring();                    //remove any limits on lure
            lureController.rb.velocity = lureController.rb.velocity / 4 + bobberGroup.rb.velocity;      //modify the lure rigid body's velocity

            rodRope.SetRopeEnd(lureController.gameObject, MarkBobberDestroy);    //reset end of the rope to the lure
            rodRope.RespawnSegments();
        }
    }

    //------------------------CASTING----------------------------------------------
    public void CastToPosition(Vector3 position, Quaternion rotation)
    {
        SetState(FishingState.Casting);                    //set state casting
        ctc.SetYRotation(rotation);     //match the rotation of the camera to the rotation of the player's camera
        castPosition = position;        //set location goal for the current cast
    }

    //-----------------------RETRACT LINE-------------------------------------------

    public void AttachFish(FishBehaviour _fish)
    {
        AudioManager.Instance.Play("FishBite");
        if (attachedFish == null)
        {
            catchDepth = gm.GetDepthLevel(gm.lureObj.transform.position.y);
            attachedFish = _fish;
            lureController.rb.useGravity = false;
        }
    }

    public void DetachFish()
    {
        attachedFish = null;
    }

    public void Retract(bool hasFish = false, bool lineBroke = false)
    {
        if (lineBroke)
        {
            //remove possibl attached fish
            attachedFish.Detach();
            attachedFish.Destroy();
            HandleFish(null);
        }
        if (!hasFish)
        {
            GameManager.Instance.SetCameraFree();           //sets the camera to free
        }
        else
        {
            HandleFish(attachedFish);                //handles the fish
            attachedFish?.Destroy();                        //despawns the fish
        }

        //tell the player to update state
        gm?.playerCont?.EndCast();

        //stop sfx
        tempReelSFX.Stop();
        AudioManager.Instance?.Stop("RodReel");

        //stop fishing
        StopFishing();
        attachedFish = null;
        gm.playerCont.EndCast();
    }

    //handles a fish being caught by the player 
    private void HandleFish(FishBehaviour fish)
    {
        string dialogue = "My line snapped...";
        //if fishAI controller is not null
        if (fish != null)
        {
            FishScriptableObject data = gm.inventoryCont.GetFishDataByID(fish.id);

            //if there is an error getting the fish
            if (data == null) { dialogue = "ERROR: Fish Does Not Exist"; }

            else
            {
                Feeshlopedia fl = Feeshlopedia.Instance;
                if (fl) fl.fishCaught(fish.id);
                else
                {
                    fl = Feeshlopedia.Instance;
                    fl.fishCaught(fish.id);
                }

                AudioManager.Instance.Play("FishFlop");

                //inventory not full
                if (gm.inventoryCont.AddFish(fish.id, fish.length)) {  dialogue = data.playerCommentary; }
                    
                //inventory full
                else { gm.ShowDialoguePlayerOneLine("<size=300%>Uh Oh...</size>"); dialogue = "Looks like my cooler is full... I have to let this go."; }

                gm.SetCameraSelf();
                fish = null;
            }
        }
        //show dialogue
        gm.ShowDialoguePlayerOneLine(dialogue);       
    }

    private void StopFishing()
    {
        gm.SetCameraShake(0); //turn off camera shake
        attachedFish = null;
        tensionAccum = 0;                       //reset tension
        RemoveBobber();                         //ensure bobber was removed
        SetState(FishingState.None);                            //set state to 0 (idle)
        lureController.ResetPositionToConnection();       //instantly snap position to idle
    }

    //-----------------------GET METHODS---------------------------------------------

    //is the lure close to a goal
    public bool GetLureCloseTo(Vector3 pos)
    {
        return Vector3.Distance(pos, lureController.rb.transform.position) <= lureStopRadius;
    }

    //-----------------------STATE METHODS--------------------------------------------


    private void SetState(FishingState _state)
    {
        state = _state;
        //set individual settings per state
        switch (_state)
        {
            case FishingState.None: //none
                lureController.rb.useGravity = true;
                lureController.isHanging = true;
                lureController.AddSpringJoint(rodTip, idleHangDistance);
                rodRope.SetSegmentCount(idleSegmentCount);
                lureController.lookAtRod = true;
                lureController.lureCollider.enabled = false;
                break;
            case FishingState.Casting: //cast
                lureController.rb.useGravity = false;
                lureController.isHanging = false;
                lureController.RemoveSpring();
                lureController.lookAtRod = true;
                rodRope.SetSegmentCount(fishingSegmentCount);
                AudioManager.Instance.Play("RodCast");
                lureController.lureCollider.enabled = true;
                break;
            case FishingState.Idle: //idle
                lureController.lookAtRod = false;
                lureController.isHanging = false;
                lureController.rb.useGravity = (attachedFish==null);
                rodRope.SetSegmentCount(fishingSegmentCount);
                bobberGroup?.SetMoveTowardsAttach(true);
                lureController.lureCollider.enabled = true;
                break;
            case FishingState.Reeling: //reeling
                tempReelSFX.volume = AudioListener.volume;
                tempReelSFX.Play();
                AudioManager.Instance.Play("RodReel");
                lureController.rb.useGravity = false;
                lureController.isHanging = false;
                lureController.lookAtRod = true;
                rodRope?.SetSegmentCount(2);
                bobberGroup?.SetMoveTowardsAttach(false);
                lureController.lureCollider.enabled = true;
                break;
            default:
                throw new System.Exception("fishing state not found");
        }
    }

    //----------------------------ACTION MAPPING -------------------------------------------

    private void BeginReel(InputAction.CallbackContext c) { if (canReel && state == FishingState.Idle) { SetState(FishingState.Reeling); } }//idle -> reeling }
    private void ContinueReel(InputAction.CallbackContext c) { }
    private void EndReel(InputAction.CallbackContext c) { if (state == FishingState.Reeling) { SetState(FishingState.Idle); tempReelSFX.Stop(); AudioManager.Instance.Stop("RodReel");   } }

    private void HoldLure(InputAction.CallbackContext c)
    {
        if (!lureController) return;
        lure_locked = true;
        if (HOLD_TOGGLE_SETTING) toggle_lure_locked = !toggle_lure_locked; 
        if (toggle_lure_locked && !attachedFish && gm && (lureController.transform.position.y <= gm.GetWaterLevel())) { lureController.AllowSinking(false); }
        else { lureController.AllowSinking(true); }
    }
    private void ReleaseLure(InputAction.CallbackContext c)
    {
        if (!toggle_lure_locked || attachedFish)
        {
            if (!canReel || !lureController) return;
            lure_locked = false;
            lureController.AllowSinking(true);
        }
    }

    //Hook Up Input Actions
    private void OnEnable()
    {
        if (!input) { return; } 
        input.actions["F_Reel"].performed += ContinueReel;
        input.actions["F_Reel"].started += BeginReel;
        input.actions["F_Reel"].canceled += EndReel;

        input.actions["F_Stop"].started += HoldLure;
        input.actions["F_Stop"].performed += HoldLure;
        input.actions["F_Stop"].canceled += ReleaseLure;
    }

    private void OnDisable()
    {
        if (!input) { return; }
        input.actions["F_Reel"].performed -= ContinueReel;
        input.actions["F_Reel"].started -= BeginReel;
        input.actions["F_Reel"].canceled -= EndReel;

        input.actions["F_Stop"].started -= HoldLure;
        input.actions["F_Stop"].performed -= HoldLure;
        input.actions["F_Stop"].canceled -= ReleaseLure;
    }

    /// <summary>
    /// Zero out the lure's velocity if the lure is being held
    /// So basically, allows the previous lure hold mechanic to work
    /// while a fish is on the line
    /// </summary>
    private void HoldLureWithFish()
    {
        if (lure_locked && attachedFish)
        {
            lureController.rb.velocity = Vector3.zero;
            if (bobberGroup) bobberGroup.rb.velocity = Vector3.zero;
        }
    }

    /// <summary>
    /// This function allows the lure to be reheld, but only if:
    /// Lure was being held, then reeled, and then reeling stopped
    /// </summary>
    private void ReapplyLureHold()
    {
        if (lure_locked && !attachedFish && gm && (lureController.transform.position.y <= gm.GetWaterLevel()))
        {
            lureController.AllowSinking(false);
        }
    }

    /// <summary>
    /// Temporary solution, a bandaid if you will, to get reel SFX 
    /// to work while we find a solution to hook it up with AudioManager.
    /// </summary>
    private void reelSFXBandaid()
    {
        tempReelSFX = gameObject.AddComponent<AudioSource>();
        tempReelSFX.loop = true;
        tempReelSFX.bypassListenerEffects = true;
        tempReelSFX.clip = (AudioClip)Resources.Load("Fishing/reel_sfx", typeof(AudioClip));
    }

    private void DoStruggleSFX()
    {
        if (attachedFish)
        {
            if (!attachedFish.isFighting) has_struggle_played = false;
            if (attachedFish.isFighting && !has_struggle_played)
            {
                has_struggle_played = true;
                AudioManager.Instance.Play("FishStruggle");
                
            }
            else if(!attachedFish.isFighting)
            {
                // Need to make it gradual. Not sure how w/ AudioManager -X
                //AudioManager.Instance.SetVolume("FishStruggle", .85f);
                AudioManager.Instance.Stop("FishStruggle");
            }
        }
        else
        {
            AudioManager.Instance.Stop("FishStruggle");
        }
    }
}
