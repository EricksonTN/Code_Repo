using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UIElements;

public class FishSpawn : MonoBehaviour
{
    // Determine how many fish have and can spawn
    public int spawn_cap = 25; // Default is 25 fish
    public bool override_spawn_cap;

    // Dimensions of the spawn box
    private MeshRenderer spawn_renderer;    //holds box for selecting size
    private Vector3 cube_dim;               //dimensions of the spawn area

    // List of Fish that can be spawned
    public GameObject fishPrefab;           //the fish prefab
    public List<int> fishIDList;            //morning fish ID list
    public List<int> noonFishIDList;        //noon fish ID list
    public List<int> nightFishIDList;       //night fish ID list
    public List<GameObject> spawnedFish;    //keeps track of fish  spawned by this box

    // Depth of the lake
    public int depth_layer;

    // Time of Day Variables
    public int morning = 5;
    public int noon = 12;
    public int night = 20;

    // Start is called before the first frame update
    private void Awake()
    {
        SetBoxDimensions();
        if(override_spawn_cap)
            spawn_cap = (int) Mathf.Ceil(Mathf.Pow(cube_dim.x * cube_dim.y * cube_dim.z, (float) 1/3)) * 2;
        PopulateFish();
    }

    /// <summary>
    /// This function grabs the dimensions of the spawn area to determine where fish can spawn within
    /// </summary>
    private void SetBoxDimensions()
    {
        // Restructure the coordinates of the spawn cube to be the corner 
        spawn_renderer = GetComponent<MeshRenderer>();
        cube_dim = spawn_renderer.bounds.size;
    }

    /// <summary>
    /// This function spawns the fish.
    /// </summary>
    public void PopulateFish()
    {
        List<int> spawnIDList = fishIDList;
        for (int i = 0; i < spawnIDList.Count; i++)
        {
            // Try to get fishSO ID from spawn box's list of fish, or default to first one in list
            FishScriptableObject fishSO;
            try { fishSO = GameManager.Instance.GetFish(spawnIDList[i]); }
            catch
            {
                Debug.Log("ERROR: Could not spawn fish ID: " + spawnIDList[i]);
                fishSO = GameManager.Instance.GetFish(spawnIDList[0]);
            }
            // float factor = fishSO.spawnFactor * spawn_cap; // TODO at a later date. To add more rarity to spawn rates, but not important as of 11/29.
            for (int j = 0; j < spawn_cap / spawnIDList.Count; j++)
            {
                SpawnFish(fishSO);
            }
        }
    }

    /// <summary>
    /// Override of the PopulateFish method. This function spawns fish based on
    /// time of day. Used by RespawnAllFish in GameManager.
    /// </summary>
    public void PopulateFish(int hour)
    {
        List<int> spawnIDList = GetIDList(hour);
        for (int i = 0; i < spawnIDList.Count; i++)
        {
            // Try to get fishSO ID from spawn box's list of fish, or default to first one in list
            FishScriptableObject fishSO;
            try { fishSO = GameManager.Instance.GetFish(spawnIDList[i]); }
            catch
            {
                Debug.Log("ERROR: Could not spawn fish ID: " + spawnIDList[i]);
                fishSO = GameManager.Instance.GetFish(spawnIDList[0]);
            }
            // float factor = fishSO.spawnFactor * spawn_cap; // TODO at a later date. To add more rarity to spawn rates, but not important as of 11/29.
            for (int j = 0; j < spawn_cap / spawnIDList.Count; j++)
            {
                SpawnFish(fishSO);
            }
        }
    }

    /// <summary>
    /// Gets ID list of fish based on times of day.
    /// Defaults to the morning ID list.
    /// </summary>
    /// <returns></returns>
    private List<int> GetIDList(int timeOfDay)
    {
        try
        {
            
            if (timeOfDay >= night) // Spawn from night fish
            {
                if (nightFishIDList.Count > 0) return nightFishIDList;
                else return fishIDList;
            }
            else if (timeOfDay >= noon) // Spawn from noon fish
            {
                if (noonFishIDList.Count > 0) return noonFishIDList;
                else return fishIDList;
            }
            else // Spawn from morning fish (which is the default)
            {
                return fishIDList;
            }
        }
        catch
        {
            Debug.Log("FAILED TO SPAWN PROPER FISH IDs. DEFAULTING...");
            return fishIDList;
        }
    }

    /// <summary>
    /// Removes A Fish From The List
    /// </summary>
    /// <param name="fish"></param>
    private void OnFishDestroyed(GameObject fish)
    {
        spawnedFish.Remove(fish);
    }

    /// <summary>
    /// Spawn a single fish based on the passed-in FishScriptableObject
    /// </summary>
    /// <param name="fishSO"></param>
    public void SpawnFish(FishScriptableObject fishSO)
    {
        //instantiate new fish
        GameObject new_fish = GameManager.Instantiate(fishPrefab, RandomSpawnPosition(), transform.rotation, GameManager.Instance.spawnedFishHolder.transform);
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
        float length = fishSO.avgLength * fishScale;
        new_fish.GetComponent<FishBehaviour>().Init(id, length, (f) => OnFishDestroyed(f));

        //add to list
        spawnedFish.Add(new_fish); 
    }

    /// <summary>
    /// Randomly generates a coordinate for a fish to spawn in, within the bounds of the spawn area
    /// </summary>
    /// <returns> Return a Vector3 representing a random coordinate within the spawn box</returns>
    private Vector3 RandomSpawnPosition()
    {
        Vector3 cube_corner_coord = new Vector3(transform.position.x - (cube_dim.x/2), transform.position.y - (cube_dim.y / 2), transform.position.z - (cube_dim.z / 2));
        Vector3 rand_pos = new Vector3(Random.Range(cube_corner_coord.x, cube_corner_coord.x + cube_dim.x), UnityEngine.Random.Range(cube_corner_coord.y, cube_corner_coord.y + cube_dim.y), Random.Range(cube_corner_coord.z, cube_corner_coord.z + +cube_dim.z));
        return rand_pos;
    }
}
