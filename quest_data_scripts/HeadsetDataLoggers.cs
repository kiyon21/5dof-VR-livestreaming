using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class HeadsetDataLogger : MonoBehaviour
{
    private List<InputDevice> devices = new List<InputDevice>();

    void Start()
    {
        InputDevices.GetDevices(devices);
        InputDevices.GetDevicesWithCharacteristics(InputDeviceCharacteristics.HeadMounted, devices);

        if (devices.Count > 0)
        {
            Debug.Log("BOMBACLATTTTTTTTTTTTTTTTTTTTTTTTTT");
            Debug.Log(devices[0].name);
            Debug.Log(devices[0].serialNumber);
        }
        else
        {
            Debug.Log("NO DEVICES FOUND");
        }

       

    }

    void Update()
    {
        if (devices.Count > 0)
        {
            InputDevice headset = devices[0];

            Vector3 position;
            if (headset.TryGetFeatureValue(CommonUsages.devicePosition, out position))
            {
                Debug.Log("Headset Position: " + position.ToString("F3"));
            }
            else
            {
                Debug.LogWarning("Failed to get headset position.");
            }

            Vector3 angularVelocity;
            if (headset.TryGetFeatureValue(CommonUsages.deviceAngularVelocity, out angularVelocity))
            {
                Debug.Log("Angular Velocity: " + angularVelocity.ToString("F3"));
            }

            Vector3 acceleration;
            if (headset.TryGetFeatureValue(CommonUsages.deviceAcceleration, out acceleration))
            {
                Debug.Log("Acceleration: " + acceleration.ToString("F3"));
            }
        }
        else
        {
            Debug.LogError("No head-mounted devices found.");
            DetectDevices();
        }
    }

    void DetectDevices()
    {
        // Clear the list to avoid duplications
        devices.Clear();

        // Define the characteristics for head-mounted devices (HMDs)
        InputDeviceCharacteristics desiredCharacteristics = InputDeviceCharacteristics.HeadMounted;

        // Get the list of devices that match the desired characteristics
        InputDevices.GetDevicesWithCharacteristics(desiredCharacteristics, devices);

        // Log the number of devices found
        Debug.Log("Number of head-mounted devices found: " + devices.Count);

        // Print the list of devices found
        foreach (var device in devices)
        {
            Debug.Log("Device found: " + device.name + " - Serial Number: " + device.serialNumber);

            // Additional check for Oculus specific characteristics if needed
            if (device.name.Contains("Oculus") || device.name.Contains("Quest"))
            {
                Debug.Log("Oculus Device detected: " + device.name);
            }
        }
    }

}