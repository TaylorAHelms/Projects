using UnityEngine;

//Player Movement

public class PlayerMovement : MonoBehaviour
{

    public Rigidbody rb;

    public float sidewaysForce = 100f;//amount of force applied when movement key is pressed

    void Update()
    {

        if (Input.GetKey("left") || Input.GetKey("a"))
            rb.AddForce(-sidewaysForce * Time.deltaTime, 0, 0, ForceMode.VelocityChange);

        if (Input.GetKey("right") || Input.GetKey("d"))
            rb.AddForce(sidewaysForce * Time.deltaTime, 0, 0, ForceMode.VelocityChange);
    }
}//class











