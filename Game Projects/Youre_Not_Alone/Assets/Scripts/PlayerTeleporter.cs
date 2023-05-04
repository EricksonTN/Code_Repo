using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerTeleporter : MonoBehaviour
{
    private GameObject currentTeleporter;

    CharacterController2D controller;

    public LevelLoader loader;

    private float x;
    private float y;

    private void Awake()
    {
        controller = this.gameObject.GetComponent<CharacterController2D>();
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        GameObject tp = collision.gameObject;

        if (collision.CompareTag("Teleporter"))
        {
            var position = collision.gameObject.GetComponent<Teleporter>().GetDestination().position;
            x = position.x;
            y = position.y;

            loader.LoadAfterTeleport();
        }
    }

    public void TelportPlayer()
    {
        transform.position = new Vector2(x, y);
    }
}
