using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AbilityEnabler : MonoBehaviour
{

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.tag == "DoubleJumpMarker")
        {
            CharacterController2D.doubleJumpEnabled = true;
        } else if (collision.tag == "DashMarker")
        {
            CharacterController2D.dashEnabled = true;
        }
    }

}
