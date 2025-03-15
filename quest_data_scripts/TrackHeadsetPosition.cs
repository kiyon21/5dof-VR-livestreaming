using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class TrackHeadsetPosition : MonoBehaviour
{
    private List<XRInputSubsystem> inputSubsystems = new List<XRInputSubsystem>();

    void Start()
    {
        // Get all available XRInputSubsystems
        SubsystemManager.GetInstances(inputSubsystems);
    }

    void Update()
    {
        if (inputSubsystems.Count > 0)
        {
            // Assuming the first XRInputSubsystem is the one we're using
            XRInputSubsystem inputSubsystem = inputSubsystems[0];

            if (inputSubsystem != null && inputSubsystem.running)
            {
                // Get the headset device
                InputDevice headset = InputDevices.GetDeviceAtXRNode(XRNode.Head);

                if (headset.isValid)
                {
                    if (headset.TryGetFeatureValue(CommonUsages.batteryLevel, out float bombaclat))
                    {

                        Debug.Log("Headset Battery: " + bombaclat);
                    }
                    // Try to get the position of the headset
                    if (headset.TryGetFeatureValue(CommonUsages.devicePosition, out Vector3 position))
                    {
                        
                        Debug.Log("Headset Position: " + position);
                    }
                    else
                    {
                        Debug.Log("Failed to get headset position.");
                    }
                }
                else
                {
                    Debug.Log("Headset device is not valid.");
                }
            }
            else
            {
                Debug.Log("XR Input Subsystem not running.");
            }
        }
        else
        {
            Debug.Log("No XR Input Subsystems found.");
        }
    }
}