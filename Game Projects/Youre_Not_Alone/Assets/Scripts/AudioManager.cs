using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

[System.Serializable]
public class Sound
{
    public string name;
    public AudioClip clip;

    private AudioSource source;    

    private bool pauseVolumeSet;

    [Range(0, 1f)]
    public float volume = 0.7f;

    [Range(0.5f, 1.5f)]
    public float pitch = 1f;

    public bool loop = false;

    public void SetSource(AudioSource _source)
    {
        source = _source;
        source.clip = clip;
        
    }

    public void SetMixer(AudioMixerGroup audioMixer)
    {
        source.outputAudioMixerGroup = audioMixer;
    }

    public void Play()
    {
        Debug.Log(this.name);
        source.volume = volume;
        source.pitch = pitch;
        source.loop = loop;
        source.Play();
    }

    public void StopPlaying()
    {
        source.Stop();
    }
}


public class AudioManager : MonoBehaviour
{
    private static Dictionary<Sound, float> soundTimerDictionary;
    public static AudioManager instance;

    [SerializeField]
    Sound[] sounds;
    [SerializeField]
    public AudioMixerGroup audioMixer;

    private void Awake()
    {
        soundTimerDictionary = new Dictionary<Sound, float>();
        if (instance != null)
        {
            Debug.LogError("TOO MANY AUDIO MANAGERS");
        }
        else
        {
            instance = this;
        }

        for (int i = 0; i < sounds.Length; i++)
        {
            GameObject _go = new GameObject("Sound_" + i + "_" + sounds[i].name);
            _go.transform.SetParent(this.transform);
            sounds[i].SetSource(_go.AddComponent<AudioSource>());
            sounds[i].SetMixer(audioMixer);
            if (sounds[i].name == "Walking (Grass)")
            {
                soundTimerDictionary[sounds[i]] = 0f;
            }
        }

    }

    private void FixedUpdate()
    {
        for (int i = 0; i < sounds.Length; i++)
        {
            if(sounds[i].name == "Wind")
            {
                if (CaveBarrier.inCave)
                {
                    sounds[i].StopPlaying();
                }
            }
        }

    }

    public void playCaveMusic()
    {
        for (int i = 0; i < sounds.Length; i++)
        {
            if (sounds[i].name == "BGM")
            {  
                    sounds[i].StopPlaying();    
            }

            else if (sounds[i].name == "FriendMusic")
            {
                    sounds[i].StopPlaying();
                AudioZone.FriendMusicIsPlaying = false;
            }
        }
        
        PlaySound("CaveMusic");
    }

    public void playParentMusic()
    {
        for (int i = 0; i < sounds.Length; i++)
        {
            if (sounds[i].name == "BGM")
            {
                sounds[i].StopPlaying();
            }

            else if (sounds[i].name == "FriendMusic")
            {
                sounds[i].StopPlaying();
                AudioZone.FriendMusicIsPlaying = false;
            }
            else if (sounds[i].name == "CaveMusic")
            {
                sounds[i].StopPlaying();
                AudioZone.CaveMusicIsPlaying = false;
            }
        }

        PlaySound("ParentMusic");
    }

    public void playFriendMusic()
    {
        for (int i = 0; i < sounds.Length; i++)
        {
            if (sounds[i].name == "BGM")
            {
                sounds[i].StopPlaying();
            }

            else if (sounds[i].name == "CaveMusic")
            {
                sounds[i].StopPlaying();
                AudioZone.CaveMusicIsPlaying = false;
            }
        }

        PlaySound("FriendMusic");
    }

    public void PlaySound(string _name)
    {
        for (int i = 0; i < sounds.Length; i++)
        {
            
            if (sounds[i].name == _name)
            {
                
                if (sounds[i].name == "Walking (Grass)")
                {
                    if (CanPlaySound(sounds[i]))
                    {
                        sounds[i].Play();
                    }
                    return;

                }
                sounds[i].Play();
                return;
            }
        }
    }

    private bool CanPlaySound(Sound sound)
    {
        float lastTimePlayed = soundTimerDictionary[sound];
        float playerMoveTimerMax = 0.543f;
        if (lastTimePlayed + playerMoveTimerMax < Time.time)
        {
            soundTimerDictionary[sound] = Time.time;
            return true;
        }
        else
        {
            return false;
        }
    }
    //break;
}
