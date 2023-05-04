using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyProjectile : MonoBehaviour
{
    public float dieTime;

    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(countdownTimer());
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.tag == "Projectile" || collision.tag == "CheckPoint")
        {

        }
        else
        {
            Die();
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    IEnumerator countdownTimer()
    {
        yield return new WaitForSeconds(dieTime);

        Die();    
    }

    void Die()
    {
        Destroy(gameObject);
    }
}
