using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;

public class EndPauseScript : MonoBehaviour
{
    private PlayerInputActions playerInputActions;

    // Start is called before the first frame update
    void Start()
    {
        playerInputActions = new PlayerInputActions();

        playerInputActions.EndScreen.Enable();
        playerInputActions.EndScreen.CloseGame.performed += QuitGame;
    }

    public void QuitGame(InputAction.CallbackContext context)
    {
        if (context.performed)  // true if the button was just hit
        {
            Debug.Log("Quit Game");
            Application.Quit();
        }
    }
}
