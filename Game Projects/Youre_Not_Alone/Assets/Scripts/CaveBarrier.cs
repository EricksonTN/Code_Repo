using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CaveBarrier : MonoBehaviour
{

    public static bool inCave = false;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.tag == "CaveEntrance")
        {
            if (inCave)
                inCave = false;
            else
                inCave = true;
        }
    }
}
