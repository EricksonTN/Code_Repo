using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AdjustTension : MonoBehaviour
{
    FishingController fishcont;
    public Slider slider;
    public Image sliderFill;
    public Color startColor;
    public Color endColor;
    private float maxSliderValue = 0.9f;
    // Start is called before the first frame update
    void Start()
    {
        fishcont = GameManager.Instance.fishingCont;
        slider.value = 0;
    }

    // Update is called once per frame
    void Update()
    {
        float mathTension = (1 / Mathf.Exp(-fishcont.tensionAccum / fishcont.tensionMax)) - 1;
        Color tensionColor = Color.Lerp(startColor, endColor, mathTension);
        if (fishcont.attachedFish)
        {
            float t = fishcont.tensionAccum / fishcont.tensionMax;
            float value = Mathf.Lerp(0, maxSliderValue, t);
            slider.value = value; // Adjust Hook's fill slider
        }
        else
        {
            slider.value = 0;
        }
        sliderFill.color = tensionColor; // Adjust Hook's fill slider's color
    }
}
