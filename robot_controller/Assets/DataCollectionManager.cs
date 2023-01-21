using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DataCollectionManager : MonoBehaviour
{
    public GameObject accelerator;
    public GameObject wheel;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        float speed = accelerator.transform.position.z;
        float steeringRotation = wheel.transform.localRotation.eulerAngles.x;
        Debug.Log(speed + " " + steeringRotation);
    }
}
