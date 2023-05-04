using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Yarn.Unity;
using UnityEngine.UI;
using Cinemachine;

public class DialogueEventsManager : MonoBehaviour
{
    public DialogueRunner dr;
    private DialogueUI2 dialogueUI;
    public DialogueControls dialogueControls;
    public GameObject Timeline_Merch;
    public GameObject Timeline_BF_Lie;
    public GameObject Timeline_BF_Truth;

    public GameObject Timeline_Parent_InKitchen;
    public GameObject Timeline_Parent_DiningRoom_01;
    public GameObject Timeline_Parent_DiningRoom_02;
    public GameObject Timeline_Parent_DiningRoom_03;

    public GameObject merch;
    public GameObject merch0;
    public GameObject merch2;
    public GameObject merch3;
    public GameObject merch5;

    public GameObject friend1;
    public GameObject friend2;
    public GameObject friend3;
    public GameObject friend4;
    public GameObject friend5;
    public GameObject friend6;

    public GameObject timelineBall;

    public GameObject sky;

    private bool didPlayerLie = false;

    public PlayerMovement movement;
    private CharacterController2D movement2D;

    private Camera cam;

    public GameObject teleporter0;
    public GameObject merchantWall;
    public GameObject friendWall;
    public GameObject friendWall2;
    public GameObject motherWall;

    public Image merchantCupImage;
    public bool hasMerchantCup = false;
    public Image ballImage;
    public bool hasBall = false;
    public Image leakImage;
    public bool hasLeak = false;

    public LevelLoader loader;

    public Animator playerAnimator;
    private CinemachineSwitcher switcher;

    private void Awake()
    {
        playerAnimator = GameObject.Find("Character Body").GetComponent<Animator>();
        movement2D = FindObjectOfType<CharacterController2D>();
        dialogueUI = FindObjectOfType<DialogueUI2>();
        switcher = FindObjectOfType<CinemachineSwitcher>();
        cam = Camera.main;
        merchantCupImage.enabled = false;
        ballImage.enabled = false;
        leakImage.enabled = false;
    }

    // Start is called before the first frame update
    void Start()
    {
        // Merchant stuff
        dr.AddCommandHandler("enable_teleporter0", enableTeleporter0);

        dr.AddCommandHandler("startFirstCutscene", startFirstCutscene);
        dr.AddCommandHandler("endFirstCutscene", endFirstCutscene);

        dr.AddCommandHandler("disableMerchantWall", disableMerchantWall);

        // Friend stuff
        dr.AddCommandHandler("playerLied", playerLied);
        dr.AddCommandHandler("disable_friendWall", disableFriendWall);

        dr.AddCommandHandler("startSecondCutscene", startSecondCutscene);
        dr.AddCommandHandler("endSecondCutscene", endSecondCutscene);

        dr.AddCommandHandler("disableFriendWall2", disableFriendWall2);

        // Mother stuff
        dr.AddCommandHandler("disableMotherWall", disableMotherWall);

        dr.AddCommandHandler("enableMerchantAndBF", enableMerchantAndBF);

        dr.AddCommandHandler("start1stMotherCutscene", start1stMotherCutscene);
        dr.AddCommandHandler("start2ndMotherCutscene", start2ndMotherCutscene);
        dr.AddCommandHandler("start3rdMotherCutscene", start3rdMotherCutscene);
        dr.AddCommandHandler("start4thMotherCutscene", start4thMotherCutscene);

        dr.AddCommandHandler("end1stMotherCutscene", end1stMotherCutscene);
        dr.AddCommandHandler("end2ndMotherCutscene", end2ndMotherCutscene);
        dr.AddCommandHandler("end3rdMotherCutscene", end3rdMotherCutscene);
        dr.AddCommandHandler("end4thMotherCutscene", end4thMotherCutscene);

        // Everything
        dr.AddCommandHandler("setOptionCountTo3", setOptionCountTo3);
        dr.AddCommandHandler("setOptionCountTo2", setOptionCountTo3);

        dr.AddFunction("checkObtainedCup", 1, delegate (Yarn.Value[] parameters)
        {
            if (hasMerchantCup == true)
            {
                merchantCupImage.enabled = false;
                hasMerchantCup = false;
                return 1;
            }
            else
            {
                return 0;
            }
        });

        dr.AddFunction("checkObtainedBall", 1, delegate (Yarn.Value[] parameters)
        {
            if (hasBall == true)
            {
                ballImage.enabled = false;
                hasBall = false;
                return 1;
            }
            else
            {
                return 0;
            }
        });

        dr.AddFunction("checkObtainedLeak", 1, delegate (Yarn.Value[] parameters)
        {
            if (hasLeak == true)
            {
                leakImage.enabled = false;
                hasLeak = false;
                return 1;
            }
            else
            {
                return 0;
            }
        });
    }

    IEnumerator delayEnableCutscene1()
    {
        yield return new WaitForSeconds(1f);

        Timeline_Merch.SetActive(true);
        movement.enableDialogueProgress();
    }

    IEnumerator delayEnableCutscene2()
    {
        yield return new WaitForSeconds(1f);

        timelineBall.SetActive(true);
        if (didPlayerLie)
        {
            Timeline_BF_Lie.SetActive(true);
            switcher.FriendCutsceneCam();
        }
        else
        {
            Timeline_BF_Truth.SetActive(true);
            switcher.FriendCutsceneCam();
        }
        movement.enableDialogueProgress();
    }

    IEnumerator delayDisableCutscene1()
    {
        yield return new WaitForSeconds(1f);

        /*
        animator.Rebind();
        animator.Update(0f);
        */
        merch3.SetActive(false);
        merch2.SetActive(true);
        Timeline_Merch.SetActive(false);

        //enable controls
    }

    IEnumerator delayDisableCutscene2()
    {
        yield return new WaitForSeconds(1f);

        Timeline_BF_Lie.SetActive(false);
        Timeline_BF_Truth.SetActive(false);

        //Animator currentAnimator = GameObject.Find("Character Body").GetComponent<Animator>();
        //currentAnimator.runtimeAnimatorController = playerAnimator.runtimeAnimatorController;

        this.GetComponent<Animator>().runtimeAnimatorController = playerAnimator.runtimeAnimatorController;
        /*
        animator = GameObject.Find("Character Body").GetComponent<Animator>();
        animator.Rebind();
        animator.Update(0f);
        */
        timelineBall.SetActive(false);

        if (didPlayerLie)
        {
            friend4.SetActive(false);
            //Timeline_BF_Lie.SetActive(false);
            switcher.ZoomOut();
        }
        else
        {
            friend3.SetActive(false);
            //Timeline_BF_Truth.SetActive(false);
            switcher.ZoomOut();
        }

        friend5.SetActive(true);
        /*
        animator = 
        GameObject.Find("Character Body").GetComponent<Animator>() = playerAnimator;
        */
        //enable controls
    }


    private void enableTeleporter0(string[] arr)
    {
        teleporter0.SetActive(true);
        merch.SetActive(false);
        merch0.SetActive(true);
    }

    private void startFirstCutscene(string[] arr)
    {
        movement2D.flipIfFacingLeft();
        // start transition
        loader.Transition();
        // Disable controls
        movement.disableDialogueProgress();

        StartCoroutine(delayEnableCutscene1());
    }

    private void startSecondCutscene(string[] arr)
    {
        movement2D.flipIfFacingLeft();
        // start transition
        loader.Transition();
        // Disable controls
        movement.disableDialogueProgress();

        StartCoroutine(delayEnableCutscene2());
    }

    private void endFirstCutscene(string[] arr)
    {
        // start transition
        loader.Transition();
        // Disable controls

        StartCoroutine(delayDisableCutscene1());
    }

    private void endSecondCutscene(string[] arr)
    {
        // start transition
        loader.Transition();
        // Disable controls

        StartCoroutine(delayDisableCutscene2());
    }

    public void setOptionCountTo3(string[] arr)
    {
        /*
        dialogueControls = FindObjectOfType<DialogueControls>();
        dialogueControls.SetOptions(3);
        */
    }

    public void setOptionCountTo2(string[] arr)
    {
        /*
        dialogueControls = FindObjectOfType<DialogueControls>();
        dialogueControls.SetOptions(2);
        */
    }

    public void disableFriendWall(string[] arr)
    {
        friendWall.SetActive(false);
        friend1.SetActive(false);
        friend2.SetActive(true);
        sky.SetActive(false);

        if (didPlayerLie)
        {
            Debug.Log("Disabled truth NPC");
            friend3.SetActive(false);
        }
        else
        {
            Debug.Log("Disabled lie NPC");
            friend4.SetActive(false);
        }
    }

    public void disableMerchantWall(string[] arr)
    {
        /*
        animator.Rebind();
        animator.Update(0f);
        */
        merchantWall.SetActive(false);
        movement2D.EnableDoubleJump();
    }

    public void disableFriendWall2(string[] arr)
    {
        /*
        animator.Rebind();
        animator.Update(0f);
        */
        friendWall2.SetActive(false);
        movement2D.EnableDash();
    }

    public void disableMotherWall(string[] arr)
    {
        motherWall.SetActive(false);
    }

    public void playerLied(string[] arr)
    {
        didPlayerLie = true;

        Debug.Log("Player lied");
    }

    public void setMerchantCup(bool value)
    {
        hasMerchantCup = value;

        if (value)
        {
            merchantCupImage.enabled = true;
        }
        else
        {
            merchantCupImage.enabled = false;
        }
    }
    public bool getMerchantCup()
    {
        return hasMerchantCup;
    }

    public void setBall(bool value)
    {
        hasBall = value;

        if (value)
        {
            Debug.Log("Enabled");
            ballImage.enabled = true;
        }
        else
        {
            Debug.Log("Disabled");
            ballImage.enabled = false;
        }
    }
    public bool getBall()
    {
        return hasBall;
    }

    public void setLeak(bool value)
    {
        hasLeak = value;

        if (value)
        {
            leakImage.enabled = true;
        }
        else
        {
            leakImage.enabled = false;
        }
    }
    public bool getLeak()
    {
        return hasLeak;
    }

    private void start1stMotherCutscene(string[] arr)
    {
        movement2D.flipIfFacingLeft();
        // start transition
        loader.Transition();
        // Disable controls
        movement.disableDialogueProgress();

        StartCoroutine(delayEnableMotherCutscene1());
    }

    IEnumerator delayEnableMotherCutscene1()
    {
        yield return new WaitForSeconds(1f);

        switcher.ParentCutsceneCam();
        Timeline_Parent_InKitchen.SetActive(true);
        movement.enableDialogueProgress();
    }

    private void end1stMotherCutscene(string[] arr)
    {
        Timeline_Parent_InKitchen.SetActive(false);
    }

    private void start2ndMotherCutscene(string[] arr)
    {
        Timeline_Parent_DiningRoom_01.SetActive(true);
    }


    private void end2ndMotherCutscene(string[] arr)
    {
        Timeline_Parent_DiningRoom_01.SetActive(false);
    }

    private void start3rdMotherCutscene(string[] arr)
    {
        Timeline_Parent_DiningRoom_02.SetActive(true);
    }


    private void end3rdMotherCutscene(string[] arr)
    {
        Timeline_Parent_DiningRoom_02.SetActive(false);
    }

    private void start4thMotherCutscene(string[] arr)
    {
        Timeline_Parent_DiningRoom_03.SetActive(true);
    }


    private void end4thMotherCutscene(string[] arr)
    {
        Timeline_Parent_DiningRoom_03.SetActive(false);
        loader.LoadNextLevel();
    }

    public void enableMerchantAndBF(string[] arr)
    {
        merch5.SetActive(true);
        friend6.SetActive(true);
    }
}
