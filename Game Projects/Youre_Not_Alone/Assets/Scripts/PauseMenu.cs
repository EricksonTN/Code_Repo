using System.Collections;
using System.Collections.Generic;
using UnityEngine.InputSystem;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.UI;

public class PauseMenu : MonoBehaviour
{
    public static bool gameIsPaused = false;
    private PlayerInputActions inputActions;
    public GameObject pauseMenuUI;

    public AudioMixer audioMixer;

    private int optionSelected;

    public Text resume;
    public Text volume;
    public Text quit;

    public Slider volumeSlider;

    private void Start()
    {
        optionSelected = 0;
    }

    public void Pause(InputAction.CallbackContext context, PlayerInputActions input)
    {
        if (context.performed)
        {
            inputActions = input;

            inputActions.PauseMenu.Resume.performed += ResumePause;
            inputActions.PauseMenu.MoveUp.performed += MoveUpPause;
            inputActions.PauseMenu.MoveDown.performed += MoveDownPause;
            inputActions.PauseMenu.VolumeUp.performed += VolumeUp;
            inputActions.PauseMenu.VolumeDown.performed += VolumeDown;
            inputActions.PauseMenu.SelectOption.performed += SelectOption;

            if (gameIsPaused)
            {
                Resume();
            }
            else
            {
                Pause();
            }
        }
    }

    public void ResumePause(InputAction.CallbackContext context)
    {
        Resume();
    }

    public void MoveUpPause(InputAction.CallbackContext context)
    {
        if (optionSelected == 0)    // Resume is selected
        {
            // Do nothing
        }
        else if (optionSelected == 1)   // Volume is selected
        {
            // Disable volume bold
            volume.text = volume.text.Substring(3, 6);

            // Enable resume bold
            resume.text = "<b>" + resume.text + "</b>";

            optionSelected = 0;
        }
        else if (optionSelected == 2)   // Quit is selected
        {
            // Disable quit bold
            quit.text = quit.text.Substring(3, 4);

            // Enable volume bold
            volume.text = "<b>" + volume.text + "</b>";

            optionSelected = 1;
        }
    }

    public void MoveDownPause(InputAction.CallbackContext context)
    {
        if (optionSelected == 0)    // Resume is selected
        {
            // Disable resume bold
            resume.text = resume.text.Substring(3, 6);

            // Enable volume bold
            volume.text = "<b>" + volume.text + "</b>";

            optionSelected = 1;
        }
        else if (optionSelected == 1)   // Volume is selected
        {
            // Disable volume bold
            volume.text = volume.text.Substring(3, 6);

            // Enable quit bold
            quit.text = "<b>" + quit.text + "</b>";

            optionSelected = 2;
        }
        else if (optionSelected == 2)   // Quit is selected
        {
            // Do nothing
        }
    }

    public void VolumeDown(InputAction.CallbackContext context)
    {
        if (optionSelected == 1)
        {
            volumeSlider.value = volumeSlider.value - 5;
        }
    }

    public void VolumeUp(InputAction.CallbackContext context)
    {
        if (optionSelected == 1)
        {
            volumeSlider.value = volumeSlider.value + 5;
        }
    }

    public void SelectOption(InputAction.CallbackContext context)
    {
        if (optionSelected == 0)    // Resume is selected
        {
            Resume();
        }
        else if (optionSelected == 1)   // Volume is selected
        {
            // Does nothing
        }
        else if (optionSelected == 2)   // Quit is selected
        {
            QuitGame();
        }
    }

    public void Resume()
    {
        optionSelected = 0;
        resume.text = "<b>Resume</b>";
        volume.text = "Volume";
        quit.text = "Quit";

        inputActions.PauseMenu.Disable();
        inputActions.Player.Enable();

        pauseMenuUI.SetActive(false);
        Time.timeScale = 1;
        gameIsPaused = false;
    }

    void Pause()
    {
        inputActions.Player.Disable();
        inputActions.PauseMenu.Enable();

        pauseMenuUI.SetActive(true);
        Time.timeScale = 0;
        gameIsPaused = true;
    }

    public void SetVolume(float volume)
    {
        audioMixer.SetFloat("Volume", volume);
    }

    public void QuitGame()
    {
        Application.Quit();
    }
}
