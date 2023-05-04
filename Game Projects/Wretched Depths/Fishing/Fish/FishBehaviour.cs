using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Events;

[RequireComponent(typeof(Rigidbody))]
public class FishBehaviour : MonoBehaviour
{
    private enum State  //enum representing current fish state
    {
        Idle,
        FollowLure,
        Attached,
    }

    private enum IdleState
    {
        Swim,
        Rest,
        Reverse
    }
    
    //SERIALIZED VARIABLES
    public bool detectWaterLevel = true;
    public LayerMask lureMask;
    public Transform localFishLocation;
    [SerializeField] private ParticleSystem particleBubbles;

    //private fish Settings
    private float swimRange = 3;              //distance the fish swims within
    private float idleTime = 5;
    private float lureDetectRadius = 5;
    private float lureAttachRadius = 0.15f;     
    private float fightTime = 0f;                   //average time for fighting                 
    private float restTime = 0f;                    //average time for resting
    private float baseRestTime;
    private float reverseTime = 1f;

    private static readonly float optimizationDistance = 30f;


    //Fish Information
    public int id { get; private set; } = 0;                     //id of the current fish
    public float length { get; private set; } = 0;               //length of this fish
    public float scale { get; private set; } = 0;
    //Private Fish Information
    private float fight = 0f;               //The intensity of the fish when fighting
    private float fishSpeed = 1f;           //The base swim speed
    private float swimAccel = 50;           //acceleration


    //references
    private FishingController fishingController;
    private LureController lureController;
    private Rigidbody rb;
    public delegate void OnFishDestroyed(GameObject fish);
    private OnFishDestroyed onFishDestroyed = delegate { };

    //timers
    private UnityTimer fightTimer;
    private UnityTimer restTimer;
    private UnityTimer idleTimer;

    //state variables
    public bool isFighting { get; private set; } = false;
    private bool isSleeping = false;
    private bool isNearSurface { get { if (!detectWaterLevel) return false; return transform.position.y >= GameManager.Instance.depthLevels[0] - 4; } }
    private bool isOutsideRange { get { return Vector3.Distance(transform.position, home) > swimRange; } }

    //VECTORS
    private Vector3 goalForward     = Vector3.zero;
    private Vector3 home            = Vector3.zero;
    private Vector3 storedForward   = Vector3.zero;

    //STATE VARIABLES
    private IdleState stateIdle = IdleState.Rest;
    private State state = State.Idle;

    private float currentSpeed;
    private int swimCount;

    private void OnDrawGizmosSelected()
    {
        //draw a black wire sphere to represent the swim range of the fish
        Gizmos.color = Color.black;
        Gizmos.DrawWireSphere(home, swimRange);
        //draw a small sphere to represent the mouth of the fish
        switch (stateIdle)
        {
            case IdleState.Swim:
                Gizmos.color = Color.green;
                break;
            case IdleState.Reverse:
                Gizmos.color = Color.yellow;
                break;
            case IdleState.Rest:
                Gizmos.color = Color.red;
                break;
            default:
                Gizmos.color = Color.blue;
                break;
        }
        Gizmos.DrawSphere(transform.position, lureAttachRadius);
        Gizmos.color = Color.green;
        Gizmos.DrawWireSphere(transform.position, lureDetectRadius);
        Gizmos.color = Color.blue;
        if(rb) Gizmos.DrawRay(transform.position, rb.velocity);
    }

    //for initializing the fish
    //if adding more variables, add them here
    //we can overload this method as well to add more complexity
    public void Init(int id, float length, OnFishDestroyed onFishDestroyed = null)
    {
        this.id = id;
        this.length = length;
        this.fightTime = GameManager.Instance.GetFish(id).baseFightTime;
        this.restTime = GameManager.Instance.GetFish(id).baseRestTime;
        this.baseRestTime = GameManager.Instance.GetFish(id).baseRestTime;
        this.fight = GameManager.Instance.GetFish(id).fightForce;
        //Debug.Log(this.fight);
        if (onFishDestroyed != null) this.onFishDestroyed = onFishDestroyed;
    }

    //PUBLIC HELPERS
    public void Detach() { state = State.Idle; stateIdle = IdleState.Rest; }    //For external use when line is snapped.
    public void Destroy() { onFishDestroyed(gameObject); Destroy(gameObject); }

    //PRIVATE HELPERS
    private void OnFightTimer() { isFighting = false; storedForward = transform.forward; }
    private void OnRestTimer() { isFighting = true; storedForward = transform.forward; }
    private void SetHomeGoalForward() { goalForward = (home - transform.position).normalized; }
    private void SetRandomGoalForward() { goalForward = Util.RandomDirXYZ(0.45f); }
    private void RotateTowardsGoalForward(Vector3 _goalForward) { goalForward = _goalForward.normalized; RotateTowardsGoalForward(); }
    private void Swim(Vector3 dir, float speed) { rb.velocity = Vector3.MoveTowards(rb.velocity, dir * speed, Time.deltaTime * swimAccel); }
    private void SwimForward() { Swim(transform.forward, currentSpeed); }
    private void SwimBackward() { Swim(-transform.forward, currentSpeed); }
    private void SwimBackward(float speed) { Swim(-transform.forward, speed); }
    private void SwimForward(float speed) { Swim(transform.forward, speed); }
    private void RotateTowardsGoalForward(UnityTimer timer = null)
    {
        float step = Time.deltaTime * 2;
        //if there is a timer, change step
        if (timer != null) { step = Time.deltaTime * Vector3.AngleBetween(storedForward, goalForward) / (timer.timeTotal / 2); }
        //rotate
        transform.forward = Vector3.RotateTowards(transform.forward, goalForward, step, 0).normalized;
    }

    void Awake()
    {
        //initialize values
        goalForward = transform.forward;
        home = transform.position;

        //add and get components
        rb = GetComponent<Rigidbody>();
        fightTimer = this.AddComponent<UnityTimer>();
        restTimer = this.AddComponent<UnityTimer>();
        idleTimer = this.AddComponent<UnityTimer>();

        if (particleBubbles) particleBubbles.Play();
    }

    private void Start()
    {
        //get references
        fishingController = GameManager.Instance.fishingCont;
        if (fishingController == null) return;
        lureController = fishingController.lureController;
    }

    private void Update()
    {
        //OPTIMIZATION
        if (optimizationDistance > 0)
        {
            float distanceToCamera = Vector3.Distance(Camera.main.transform.position, transform.position);
            if (distanceToCamera > optimizationDistance)
            {
                if (!isSleeping)
                {
                    if (particleBubbles) particleBubbles.Stop();
                    isSleeping = true;
                    rb.Sleep();
                    idleTimer.Pause();
                }
            }
            else if (isSleeping)
            {
                if (particleBubbles) particleBubbles.Play();                
                isSleeping = false;
                rb.WakeUp();
                idleTimer.Unpause();
            }
            
            //if we are sleeping, return
            if (isSleeping) return;
        } 

        //FORGET LURE
        if (fishingController != null || lureController != null) {

            //get distance to lure
            float distToLure = Vector3.Distance(lureController.transform.position, this.transform.position);

            //if following the lure
            if (state == State.FollowLure)
            {
                //LURE FORGET CONDITIONS

                //if the lure is taken by a different fish, ignore it
                if (fishingController.attachedFish != this && fishingController.attachedFish != null) { Detach(); }

                //if following lure out of the water, ignore the lure
                else if (isNearSurface) { Detach(); }

                //if the lure controller exists and we are too far away from it, ingore the lure
                else if (lureController && distToLure > lureDetectRadius * 2) { Detach(); }
            }
            
        }

        //FISH BEHAVIOUR
        switch (state)
        {
            case State.Idle:            //idling
                DoIdle();
                break;
            case State.FollowLure:      //swimming to lure
                DoFollowLure();
                break;
            case State.Attached:        //attached
                DoAttached();
                break;
            default:
                break;

        }
    }

    private void FixedUpdate()
    {
        //Changes the rest timer for each fish based on difficulty
        if (GameManager.Instance.isEasy)
        {
            this.restTime = this.baseRestTime * 1.75f;
        }
        else {this.restTime = this.baseRestTime;}
        
        if (state == State.Attached)                                                                                                       //if we are attached
        {
            Vector3 moveDir = (transform.position.y > home.y) ? -goalForward.normalized : Util.V3ToV2XZ(-goalForward).normalized;     //swim either away or away and down based on if we are above the SPAWN
            lureController.rb.AddForce(moveDir * fight * (isFighting ? 0.7f : 0.25f));                                        //swim either fast or slow based on whether or not the fish is currently fighting
        }

        //get distance to lure
        float distToLure = Vector3.Distance(lureController.transform.position, this.transform.position);
        //DETECT LURE
        //if state IS idle
        if (state == State.Idle)
        {
            //LURE RECOGNITION
            if (fishingController.attachedFish == null && Physics.CheckSphere(transform.position, lureDetectRadius, lureMask))
            {
                //FACTORS FOR DETECT CHANCE
                float baseFactor = 0.2f; //base detect factor


                //allure of the lure
                float allure = (GameManager.Instance.inventoryCont.GetCurrentLure().allure);
                
                if (allure == 0) return; //do not allure fish when allure is zero
                if (distToLure < lureAttachRadius) allure = 99; //always allure fish if really close


                //0.1 when far 1 when close
                float distFactor = Mathf.Max((1 - (distToLure / (lureDetectRadius * 1.5f))), 0);
                distFactor *= distFactor * distFactor;

                //(1-4) allure -> 
                float allureFactor = allure;


                //1 when still 0.2 when not
                float moveFactor = 1;
                if (lureController && lureController.rb.velocity.y < -0.95f) moveFactor = 0.01f;


                

                float detectChancePerSecond = baseFactor * allure * distFactor * moveFactor;
                //attempt to detect the lure

                bool debug = false;
                if (debug)
                {
                    Debug.DrawRay(transform.position + (0.25f * Vector3.up), Vector3.up, Color.white, 0.05f);
                    Debug.DrawRay(transform.position + (0.25f * Vector3.up), Vector3.up * detectChancePerSecond, Color.red, 0.05f);
                }
                

                if (Util.GetRandChance(detectChancePerSecond * Time.fixedDeltaTime)) { state = State.FollowLure; }
            }
        }
    }

    //STATE HELPERS
    private void StartIdleReverse()
    {
        idleTimer.StopTimer();
        stateIdle = IdleState.Reverse;
        //start timer
        idleTimer.StartTimer(reverseTime);
    }
    private void StartIdleSwim()
    {
        idleTimer.StopTimer();
        stateIdle = IdleState.Swim;
        //change variables
        SetRandomGoalForward();
        currentSpeed = Util.ScaleValueRandom(fishSpeed, 0.75f, 1.25f);
        //start timer
        idleTimer.StartTimer(Util.ScaleValueRandom(idleTime, 0.75f, 1.25f));
    }
    private void StartIdleRest() 
    {
        idleTimer.StopTimer();
        stateIdle = IdleState.Rest;
        //start timer
        idleTimer.StartTimer(Random.Range(idleTime * .5f, idleTime * 1.5f));
    }

    // -----ACTIONS-----
    private void DoIdle()
    {
        if (isOutsideRange || isNearSurface) SetHomeGoalForward();

        switch (stateIdle)
        {
            case IdleState.Rest:    //RESTING
                //slow down
                if(currentSpeed > 0.15f) { currentSpeed = currentSpeed - Time.deltaTime; }
                //perform swim action
                SwimForward();


                if (idleTimer.isTiming) { break; }  //if we are still doing this action, break

                //set to swim state
                StartIdleSwim();
                break;

            case IdleState.Swim:    //SWIMMING
                //perform swim action
                SwimForward();
                //rotate fish towards its goal
                RotateTowardsGoalForward(idleTimer);

                if (idleTimer.isTiming) { break; }  //if we are still doing this action, break

                //set to rest state
                StartIdleRest();
                break;

            case IdleState.Reverse:  //Backing Up
                //perform swim action backwards
                SwimBackward(fishSpeed);

                if (idleTimer.isTiming) { break; }  //if we are still doing this action, break

                //setidle to rest
                StartIdleRest();
                break;

            default:
                break;
        }

        //rb.angularVelocity = Vector3.zero;
        //Debug.DrawRay(transform.position, rb.velocity, Color.blue, 0.35f);
    }

    private void DoFollowLure()
    {
        RotateTowardsGoalForward((lureController.transform.position + lureController.rb.velocity / 2 - transform.position).normalized);
        SwimForward(fishSpeed);
        if (Physics.CheckSphere(transform.position, lureAttachRadius, lureMask)) { state = State.Attached; storedForward = transform.forward; fishingController.AttachFish(this); }
    }

    private void DoAttached()
    {
        Vector3 rodTipPos = fishingController.rodTip.transform.position;
        goalForward = rodTipPos - transform.position;
        //rotate fish mouth towards rod tip        
        storedForward = Vector3.RotateTowards(storedForward, goalForward, Time.deltaTime * 10, 0);
        //precess if fighting
        Quaternion q = Quaternion.identity;
        float x = Mathf.Sin(fightTimer.GetElapsedTime() * 15);
        float y = Mathf.Sin(restTimer.GetElapsedTime() * 5);
        q = Quaternion.Euler(0, 20 * x + 15 * y, 0);
        transform.forward = q * storedForward;
        transform.position = lureController.gameObject.transform.position;                                                        //move fish position to the lure

        bool debug = false;
        if (debug)
        {
            Debug.DrawRay(transform.position, storedForward, Color.yellow, 0.05f);
            Debug.DrawRay(transform.position, transform.forward, Color.red, 0.05f);
            Debug.DrawRay(transform.position, goalForward, Color.green, 0.05f);
        }

        if (!isFighting && !restTimer.isTiming)
            restTimer.StartTimer(Random.Range(restTime * .8f, restTime * 1.3f), () => OnRestTimer());
        if (isFighting && !fightTimer.isTiming)
            fightTimer.StartTimer(Random.Range(fightTime * .7f, fightTime * 1.2f), () => OnFightTimer());
    }

    private void OnCollisionEnter(Collision collision) 
    {
        if(stateIdle != IdleState.Reverse)
            StartIdleReverse();
    }
}
