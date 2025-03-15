using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class WebCamDisplay : MonoBehaviour
{
    public Canvas canvas;
    public RawImage rawImage;
    private WebCamTexture webCamTexture;

    void Start()
    {

        WebCamDevice[] devices = WebCamTexture.devices;
        if (devices.Length > 0)
        {


            webCamTexture = new WebCamTexture("HD Webcam eMeet C960");
            rawImage.texture = webCamTexture;
            rawImage.material.mainTexture = webCamTexture;
            webCamTexture.Play();
        }
        else
        {
            Debug.LogError("No webcam detected!");
        }
    }
}
