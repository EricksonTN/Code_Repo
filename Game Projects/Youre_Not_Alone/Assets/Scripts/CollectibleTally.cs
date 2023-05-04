using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CollectibleTally : MonoBehaviour
{
    public Image merchantCupImage;

    public bool hasMerchantCup = false;

    public void Awake()
    {
        merchantCupImage.enabled = false;
    }

    public void setMerchantCup(bool value)
    {
        hasMerchantCup = value;

        if (value)
        {
            merchantCupImage.enabled = true;
        }
        else
        {
            merchantCupImage.enabled = false;
        }
    }
    public bool getMerchantCup()
    {
        return hasMerchantCup;
    }
}
