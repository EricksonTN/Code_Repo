using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FiringEnemy : MonoBehaviour
{

    public float fireCooldown, projectileSpeed;
    private bool canShoot;

    public GameObject projectile;
    public Transform shootPos;
    [Range(-1, 1)] [SerializeField] public float firingX;
    [Range(-1, 1)] [SerializeField] public float firingY;

    // Start is called before the first frame update
    void Start()
    {
        canShoot = true;   
    }

    // Update is called once per frame
    void Update()
    {
        if(canShoot)
        StartCoroutine(Fire());
    }

    IEnumerator Fire()
    {
        canShoot = false;
        yield return new WaitForSeconds(fireCooldown);
        GameObject newProjectile = Instantiate(projectile, shootPos.position, Quaternion.identity);

        newProjectile.GetComponent<Rigidbody2D>().velocity = new Vector2(firingX * projectileSpeed, firingY * projectileSpeed);
        canShoot = true;
    }
}
