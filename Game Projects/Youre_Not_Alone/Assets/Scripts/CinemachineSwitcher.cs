using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Cinemachine;


public class CinemachineSwitcher : MonoBehaviour
{
    public CinemachineVirtualCamera vcam1; // Zoomed out
    public CinemachineVirtualCamera vcam2; // Zoomed in
    public CinemachineVirtualCamera vcam3; // Friend cutscene camera
    public CinemachineVirtualCamera vcam4; // Parent cutscene camera
    private int currentCamera;

    void Start()
    {
        vcam2.enabled = true;
        ZoomOut();
    }

    public void SwitchPriority()
    {
        if (currentCamera == 0) // Zoomed out
        {
            vcam1.Priority = 0;
            vcam2.Priority = 1;
            vcam3.Priority = 0;
            vcam4.Priority = 0;

            currentCamera = 1;
        }
        else // Zoomed in
        {
            vcam1.Priority = 1;
            vcam2.Priority = 0;
            vcam3.Priority = 0;
            vcam4.Priority = 0;

            currentCamera = 0;
        }
    }

    public void ZoomIn()
    {
        vcam1.Priority = 0;
        vcam2.Priority = 1;
        vcam3.Priority = 0;
        vcam4.Priority = 0;

        currentCamera = 1;
    }

    public void ZoomOut()
    {
        vcam1.Priority = 1;
        vcam2.Priority = 0;
        vcam3.Priority = 0;
        vcam4.Priority = 0;

        currentCamera = 0;
    }

    public void FriendCutsceneCam()
    {
        vcam1.Priority = 0;
        vcam2.Priority = 0;
        vcam3.Priority = 1;
        vcam4.Priority = 0;

        currentCamera = 2;
    }

    public void ParentCutsceneCam()
    {
        vcam1.Priority = 0;
        vcam2.Priority = 0;
        vcam3.Priority = 0;
        vcam4.Priority = 1;

        currentCamera = 3;
    }
}
