using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckpointProperties : MonoBehaviour
{
    public Sprite litLamp;
    public Sprite unlitLamp;
    public Vector2 respawnPoint;

    public Vector2 getPoint()
    {
        return respawnPoint;
    }

    public Sprite getLitLamp()
    {
        return litLamp;
    }

    public Sprite getUnlitLamp()
    {
        return unlitLamp;
    }
}