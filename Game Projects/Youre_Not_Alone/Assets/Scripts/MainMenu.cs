using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{
    public GameObject canvas;

    public LevelLoader loader;

    private PlayerInputActions playerInputActions;

    private int optionSelected;

    public Text play;
    public Text quit;

    public void Start()
    {
        optionSelected = 0;

        playerInputActions = new PlayerInputActions();
        playerInputActions.TitleScreen.Enable();
        playerInputActions.TitleScreen.MoveUp.performed += MoveUp;
        playerInputActions.TitleScreen.MoveDown.performed += MoveDown;
        playerInputActions.TitleScreen.SelectOption.performed += SelectOption;

    }

    public void MoveUp(InputAction.CallbackContext context)
    {
        if (optionSelected == 0)    // Play is selected
        {
            // Do nothing
        }
        else if (optionSelected == 1)   // Quit is selected
        {
            // Disable quit bold
            quit.text = quit.text.Substring(3, 4);

            // Enable play bold
            play.text = "<b>" + play.text + "</b>";

            optionSelected = 0;
        }
    }

    public void MoveDown(InputAction.CallbackContext context)
    {
        if (optionSelected == 0)    // Play is selected
        {
            // Disable play bold
            play.text = play.text.Substring(3, 4);

            // Enable quit bold
            quit.text = "<b>" + quit.text + "</b>";

            optionSelected = 1;
        }
        else if (optionSelected == 1)   // Quit is selected
        {
            // Do nothing
        }
    }

    public void SelectOption(InputAction.CallbackContext context)
    {
        if (optionSelected == 0)    // Play is selected
        {
            PlayGame();
        }
        else if (optionSelected == 1)   // Quit is selected
        {
            QuitGame();
        }
    }

    public void PlayGame()
    {
        playerInputActions.TitleScreen.Disable();
        loader.LoadNextLevel();
        canvas.SetActive(false);
    }

    public void QuitGame()
    {
        Application.Quit();
    }

}
