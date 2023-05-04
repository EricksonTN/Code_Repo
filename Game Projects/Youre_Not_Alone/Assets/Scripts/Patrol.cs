using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Patrol : MonoBehaviour
{
    public float speed;

    public Transform[] moveSpots;
    public int startingSpot;

    public int i;

    // Start is called before the first frame update
    void Start()
    {
        transform.position = moveSpots[startingSpot].position;
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = Vector2.MoveTowards(transform.position, moveSpots[i].position, speed * Time.deltaTime);

        if (Vector2.Distance(transform.position, moveSpots[i].position) < 0.2f)
        {
            i++;
            if(i == moveSpots.Length)
            {
                i = 0;
            }
        }
        
    }
}
