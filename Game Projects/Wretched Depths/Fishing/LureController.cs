using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using Unity.VisualScripting;
using UnityEngine;

[RequireComponent(typeof(Collider))]
[RequireComponent(typeof(Rigidbody))]
public class LureController : MonoBehaviour
{
    //public

    public PhysicMaterial pm_friction;
    public PhysicMaterial pm_frictionless;
    public GameObject meshObject;       //object containing the mesh for the lure
    public float idleHangDistance;      //default hanging distance for the lure
    public Transform rotTipT = null;     //goal to rotate towards
    public bool lookAtRod = false;
    public bool isHanging = true;
    public MeshRenderer lureRend;
    public MeshFilter lureFilter;
    public Collider lureCollider;
    public bool isAttractive = true;
    float DepthModifier = 1;

    public bool canSink { get; private set; } = true;

    //private
    private SpringJoint joint;          //dynamically created spring joint for limits
    private int texID = -1;
    private float addSinkForce = 0;
    public Rigidbody rb { get; private set; }                //rigidbody of the lure
    public Collider coll { get; private set; }              //collider of lure

    public float smoothYVelocity { get; private set; } = 0f;

    private void Awake()
    {
        rb = GetComponent<Rigidbody>();
        coll = GetComponent<Collider>();
    }

    //removes any possible existing spring joint from the lure
    public void RemoveSpring()
    {
        if (joint != null)          //if joint exists
        {
            Destroy(joint);         //remove and null it
            joint = null;
        }            
    }

    //Adds a spring joint attached to a given rb to the lure with an optional hanging distance (no distance implies its the max depth of the lure)
    public void AddSpringJoint(Rigidbody attachedRB) { AddSpringJoint(attachedRB, GameManager.Instance.inventoryCont.GetCurrentLure().maxDepth * DepthModifier); }
    public void AddSpringJoint(Rigidbody attachedRB, float hangDistance) 
    {
        RemoveSpring();                                         //destroy previous joint
        joint = this.gameObject.AddComponent<SpringJoint>();    //initialize spring values 
        joint.autoConfigureConnectedAnchor = false;
        joint.spring = 9999f;
        joint.damper = 200;
        joint.connectedBody = attachedRB;
        joint.maxDistance = hangDistance;
        joint.minDistance = 0;
    }

    private void Update()
    {
        //confirm lure id
        LureScriptableObject newLure = GameManager.Instance.inventoryCont.GetCurrentLure();
        if (newLure && texID != newLure.id)
        {

            //update lure graphics
            if (newLure.mat && newLure.mesh) 
            { 
                addSinkForce = newLure.addSinkForce; texID = newLure.id; 
                lureRend.sharedMaterial = newLure.mat; lureFilter.sharedMesh = newLure.mesh; 
            }

        }

        Quaternion goalRotation = Quaternion.identity;   //setup rotation for our lure
        if (lookAtRod)                                   //if we are looking at the rod, simply look in the direction of the rod
        {
            Vector3 toRodDir = (rotTipT.position - meshObject.transform.position).normalized;
            goalRotation = Quaternion.LookRotation(toRodDir);
        }
        else                        
        {
            goalRotation = Util.GeneralizedLookRotation(transform.forward, Vector3.up, Vector3.up, GameManager.Instance.playerCont.transform.forward);
        }
        meshObject.transform.rotation = Quaternion.Slerp(meshObject.transform.rotation, goalRotation, Time.deltaTime*10);

        //drag is either 2 or 8 based on if it is below or above the water
        rb.drag = (transform.position.y > GameManager.Instance.GetWaterLevel()) && !isHanging ? 2 : 8;


    }

    private void FixedUpdate()
    {
        FishingController fishingCont = GameManager.Instance.fishingCont;
        if (canSink && addSinkForce != 0 && !isHanging && fishingCont.isIdle && fishingCont.attachedFish == null)
        {
            rb.AddForce(Vector3.down * (addSinkForce), ForceMode.Force);
        }

        coll.sharedMaterial = (fishingCont.attachedFish != null) ? pm_frictionless : pm_friction;

        smoothYVelocity = Mathf.Lerp(smoothYVelocity, rb.velocity.y, 0.15f);
    }

    //resets the position of the lure to be directly below it's attached object
    public void ResetPositionToConnection()
    {
        if (joint)          //if the joint exists
        {
            //reset position and set velocity to zero
            gameObject.transform.position = joint.connectedBody.position + Vector3.down * idleHangDistance;
            rb.velocity = Vector3.zero;
        }
    }

    public void SetLureVisible(bool visible)
    {
        meshObject.SetActive(visible);
    }

    public void AllowSinking(bool allow)
    {
        canSink = allow;
        rb.useGravity = allow;
    }
}
