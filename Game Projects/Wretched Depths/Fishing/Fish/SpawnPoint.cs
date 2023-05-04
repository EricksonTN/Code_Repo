using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnPoint : MonoBehaviour
{
    // Input Variables
    public int fishID;
    private FishScriptableObject fishSO;
    public GameObject defaultFish;

    // Start is called before the first frame update
    private void OnEnable()
    {
        try { fishSO = GameManager.Instance.GetFish(fishID); }
        catch
        {
            Debug.Log("ERROR: Spawn Point Failed Spawn");
            fishSO = GameManager.Instance.GetFish(100);
        }
        SpawnFish(fishSO);
    }

    /// <summary>
    /// Spawn a single fish based on the passed-in FishScriptableObject
    /// </summary>
    /// <param name="fishSO"></param>
    public void SpawnFish(FishScriptableObject fishSO)
    {
        //instantiate new fish
        GameObject new_fish = GameManager.Instantiate(defaultFish, transform.position, transform.rotation, GameManager.Instance.spawnedFishHolder.transform);
        new_fish.name = fishSO.name;

        //determine scale
        float fishScale = Random.Range(fishSO.fishScale.x, fishSO.fishScale.y);

        // Add child fish object w/ mesh + rig
        if (fishSO.fishObject)
        {
            GameObject fishObject = Instantiate(fishSO.fishObject, new_fish.GetComponent<FishBehaviour>().localFishLocation);
            //fishObject.transform.localScale *= fishScale;   //update scale TODO test this

            //setup animator
            Animator anim = fishObject.GetComponent<Animator>();
            if (anim && fishSO.animatorController) anim.runtimeAnimatorController = fishSO.animatorController;
        }

        // Initialize the behaviour
        int id = fishSO.id;
        float length = fishSO.avgLength * (Mathf.Round(fishScale * 100.0f) * 0.01f);
        new_fish.GetComponent<FishBehaviour>().Init(id, length, null);
    }
}
