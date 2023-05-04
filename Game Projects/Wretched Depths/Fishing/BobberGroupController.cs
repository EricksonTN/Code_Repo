using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BobberGroupController : MonoBehaviour
{
    public Rigidbody rb;                //rigidbody of the bobber object
    public GameObject bobber;           //bobber object
    public RopeControllerVisual rope;   //rope starting from the bobber
    public float moveSpeed;             //max speed of the bobber
    public float moveAcc;               //acceleration of the bobber
    public LineRenderer fishingLine;

    private Transform attachT = null;   //transform of the attached object (end of rope)
    private Transform rodT = null;      //transform of the rod tip
    private Transform goalT = null;     //current movement goal transform

    //sets up rope connection and initializes transforms
    public void Init(GameObject attach, Transform _rodPos)
    {
        rodT = _rodPos;                 //setup rodT and attachT
        attachT = attach.transform;
        goalT = attachT;                //current goal is attach by default
        rope.SetRopeEnd(attach);        //set rope end to the lure
        rope.RespawnSegments();         //reset the rope 
    }

    //set whether the bobber should move towards the attach transform or the rod transform
    public void SetMoveTowardsAttach(bool moveTowardsAttach)
    {
        goalT = moveTowardsAttach ? attachT : rodT;
    }

    private void FixedUpdate()
    {
        //move towards the current goal if the goal is not null
        if(goalT != null)
        {
            Vector3 goalVelocity = moveSpeed * (goalT.position - bobber.transform.position).normalized;
            rb.velocity = Vector3.MoveTowards(rb.velocity, goalVelocity, Time.deltaTime * moveAcc);
        }
    }

    //returns the bobber's distance from it's current goal
    public float GetBooberGoalDistance()
    {
        return Vector2.Distance(Util.V3ToV2XZ(goalT.position), Util.V3ToV2XZ(bobber.transform.position));
    }
}
