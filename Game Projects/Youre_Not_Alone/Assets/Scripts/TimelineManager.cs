using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;

public class TimelineManager : MonoBehaviour
{
    private bool fix = false;
    public Animator playerAnimator;
    public AnimationClip idle;
    public RuntimeAnimatorController playerAnim;
    public PlayableDirector director;
    private DialogueMover dialogueMover;

    public GameObject bone6;
    public GameObject bone7;
    public GameObject bone8;
    public GameObject bone9;

    private float bone6_default_rotation_z = 173.354f;
    private float bone7_default_rotation_z = 177.756f;
    private float bone8_default_rotation_z = 5f;
    private float bone9_default_rotation_z = -18.269f;

    void OnEnable()
    {
        playerAnim = playerAnimator.runtimeAnimatorController;
        dialogueMover = FindObjectOfType<DialogueMover>();
    }

    void OnDisable()
    {
        playerAnim = playerAnimator.runtimeAnimatorController;
        UnlockPlayerTextboxY();
        FixPlayerBones();
    }

    // Update is called once per frame
    void Update()
    {
        if (director.state != PlayState.Playing && !fix)
        {
            fix = true;
            playerAnimator.runtimeAnimatorController = playerAnim;
        }
    }

    public void FixPlayerBones()
    {
        bone6.transform.rotation = Quaternion.identity;
        bone7.transform.rotation = Quaternion.identity;
        bone8.transform.rotation = Quaternion.identity;
        bone9.transform.rotation = Quaternion.identity;


        //default rotation is 267.295 for bone 6 and bone 7
        bone6.transform.eulerAngles = new Vector3(0f, 0f, 92.705f + bone6_default_rotation_z);
        bone7.transform.eulerAngles = new Vector3(0f, 0f, 92.705f + bone7_default_rotation_z);
        bone8.transform.Rotate(new Vector3(0f, 0f, bone8_default_rotation_z));
        bone9.transform.Rotate(new Vector3(0f, 0f, bone9_default_rotation_z));
    }

    public void LockPlayerTextboxY()
    {
        dialogueMover.lockPlayerTextboxY();
    }

    public void UnlockPlayerTextboxY()
    {
        dialogueMover.unlockPlayerTextboxY();
    }

    public void ParentCutscenePlaying()
    {
        dialogueMover.startedParentCutscene();
    }

    public void ParentCutscenePlaying2()
    {
        dialogueMover.startedParentCutscene2();
    }
}