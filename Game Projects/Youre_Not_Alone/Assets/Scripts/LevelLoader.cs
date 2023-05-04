using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class LevelLoader : MonoBehaviour
{
    public Animator transition;

    public PlayerTeleporter teleporter;
    public PlayerMovement movement;

    public float transitionTime = 1f;


    //Between Scene Transition

    public void LoadNextLevel()
    {
        StartCoroutine(LoadLevel(SceneManager.GetActiveScene().buildIndex + 1));
    }

    IEnumerator LoadLevel(int levelIndex)
    {
        transition.SetTrigger("Start");

        yield return new WaitForSeconds(transitionTime);

        SceneManager.LoadScene(levelIndex);
    }

    public void LoadNextLevelName(string v)
    {
        StartCoroutine(LoadLevel(v));
    }

    IEnumerator LoadLevel(string levelName)
    {
        transition.SetTrigger("Start");

        yield return new WaitForSeconds(transitionTime);

        SceneManager.LoadScene(levelName);
    }

    //Transition for Teleport

    public void LoadAfterTeleport()
    {
        StartCoroutine(LoadTeleport());
    }

    IEnumerator LoadTeleport()
    {
        transition.SetTrigger("Start");

        yield return new WaitForSeconds(transitionTime);

        transition.SetTrigger("End");
        teleporter.TelportPlayer();
    }

    public void LoadAfterDeath()
    {
        StartCoroutine(LoadRespawn());
    }

    public void Transition()
    {
        StartCoroutine(OnlyTransition());
    }

    IEnumerator OnlyTransition()
    {
        transition.SetTrigger("Start");

        yield return new WaitForSeconds(transitionTime);

        transition.SetTrigger("End");
    }

    IEnumerator LoadRespawn()
    {
        transition.SetTrigger("Start");

        yield return new WaitForSeconds(transitionTime);

        transition.SetTrigger("End");
        movement.Respawn();
        movement.setDead(false);
    }
}
