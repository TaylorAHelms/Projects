using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Obstacle Movement

public class ObstacleMovement : MonoBehaviour
{
    public Rigidbody rb;

    public float forwardForce = -2000f;//force for obstacles moving towards the player

    void Update()
    {
        rb.AddForce(0, 0, forwardForce * Time.deltaTime);
    }
}
