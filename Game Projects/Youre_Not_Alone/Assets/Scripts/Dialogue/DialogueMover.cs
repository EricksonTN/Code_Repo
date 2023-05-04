using UnityEngine;
using Yarn.Unity;

public class DialogueMover : MonoBehaviour
{
    private DialogueUI2 dialogueUI;
    private Camera cam;
    bool lockedTextboxY;
    float lastPlayerY;
    float lockedY;
    bool parentCutscenePlaying;
    bool parentCutscenePlaying2;
    public GameObject player;

    // Start is called before the first frame update
    void Awake()
    {
        // Retrieve references for the DialogueUI and Camera
        dialogueUI = FindObjectOfType<DialogueUI2>();
        cam = Camera.main;
        lockedTextboxY = false;
        lastPlayerY = 0;
        lockedY = 0;
        parentCutscenePlaying = false;
        parentCutscenePlaying2 = false;
    }

    private void Update()
    {
        // If the Camera changes view, this ensures that the Dialogue Bubble position
        // get recalculated based on the new Camera view, every frame
        SetDialogueOnTalkingCharacter();

        lastPlayerY = player.transform.position.y + 6.45f;

        /*
        if (lockedTextboxY)
        {
            Debug.Log("Textbox is locked");
        }
        else
        {
            Debug.Log("Textbox is unlocked");
        }
        */
    }

    // Retrieve the character who's talking from the text
    public void SetDialogueOnTalkingCharacter()
    {
        GameObject character;
        string line, name;

        // Get the dialogue line
        line = dialogueUI.getLineText();
        // Search for the character who's talking
        if (line.Contains(":"))
            name = line.Substring(0, line.IndexOf(":"));
        else
            //name = "Player";
            name = "Character Body";
        // Search the GameObject of the character in the Scene
        character = GameObject.Find(name);
        // Sets the dialogue position
        SetDialoguePosition(character);
    }

    public void lockPlayerTextboxY()
    {
        lockedTextboxY = true;
        lockedY = lastPlayerY;
    }

    public void unlockPlayerTextboxY()
    {
        lockedTextboxY = false;
    }

    public void startedParentCutscene()
    {
        parentCutscenePlaying = true;
    }

    public void startedParentCutscene2()
    {
        player.transform.localScale = new Vector3(.14f, .14f, .14f);
        parentCutscenePlaying = false;
        parentCutscenePlaying2 = true;
    }

    private void SetDialoguePosition(GameObject character)
    {
        float x;
        float y;

        if (character.name.Equals("Merch"))
        {
            x = character.transform.position.x + 1;
            y = character.transform.position.y + 7.5f;

        }
        else if (character.name.Equals("Merch2"))
        {
            x = character.transform.position.x - 0;
            y = character.transform.position.y + 7.5f;

        }
        else if (character.name.Equals("Merch3"))
        {
            x = character.transform.position.x - 0;
            y = character.transform.position.y + 7.5f;

        }
        else if (character.name.Equals("Merch5"))
        {
            x = character.transform.position.x + .1f;
            y = character.transform.position.y + 7.5f;

        }
        else if (character.name.Contains("Character Body"))
        {
            if (!parentCutscenePlaying)
            {
                if (!parentCutscenePlaying2)
                {
                    if (lockedTextboxY)
                    {
                        x = character.transform.position.x;
                        y = lockedY;
                    }
                    else
                    {
                        if (character.transform.position.y + 6.45f > lastPlayerY)
                        {
                            x = character.transform.position.x;
                            y = character.transform.position.y + 6.45f;
                        }
                        else
                        {
                            x = character.transform.position.x;
                            y = lastPlayerY;
                        }
                    }
                }
                else
                {
                    //Debug.Log("Cutscene 2 is playing");
                    if (lockedTextboxY)
                    {
                        x = character.transform.position.x;
                        y = lockedY + .5f;
                    }
                    else
                    {
                        if (character.transform.position.y + 6.45f > lastPlayerY)
                        {
                            x = character.transform.position.x;
                            y = character.transform.position.y + 6.45f + .5f;
                        }
                        else
                        {
                            x = character.transform.position.x;
                            y = lastPlayerY + .5f;
                        }
                    }
                }
            }
            else
            {
                //Debug.Log("Cutscene 1 is playing");
                if (lockedTextboxY)
                {
                    x = character.transform.position.x;
                    y = lockedY + .5f;
                }
                else
                {
                    if (character.transform.position.y + 6.45f > lastPlayerY)
                    {
                        x = character.transform.position.x;
                        y = character.transform.position.y + 6.95f;
                    }
                    else
                    {
                        x = character.transform.position.x;
                        y = lastPlayerY + .5f;
                    }
                }
            }
        }
        else if (character.name.Contains("BF6"))
        {

            x = character.transform.position.x + .75f;
            y = character.transform.position.y + 2.35f;
        }
        else if (character.name.Contains("BF"))
        {

            x = character.transform.position.x - .75f;
            y = character.transform.position.y + 2.35f;
        }
        else if (character.name.Contains("Parent"))
        {
            x = character.transform.position.x +.50f;

            if (!parentCutscenePlaying)
            {
                if (parentCutscenePlaying2)
                {
                    y = character.transform.position.y + 8f;
                }
                else
                {
                    y = character.transform.position.y + 7.5f;
                }
            }
            else
            {
                y = character.transform.position.y + 8f;
            }
        }
        else
        {
            x = character.transform.position.x + 1;
            y = character.transform.position.y + 7.5f;
        }

        // Retrieve the position where the top part of the sprite is in the world
        //float characterSpriteHeight = character.GetComponent<SpriteRenderer>().sprite.bounds.extents.y;

        // Create position with the sprite top location
        Vector2 characterPosition = new Vector2(x, y);

        // Set the DialogueBubble position to the sprite top location in Screen Space
        this.transform.position = cam.WorldToScreenPoint(characterPosition);
    }
}