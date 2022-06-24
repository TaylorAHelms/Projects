using UnityEngine;

//Camera Controller
//Camera follows the player from behind and slightly above

public class CameraControl : MonoBehaviour
{


    public Transform player;

    public Vector3 offset = new Vector3(0, 1, -5);

    void Update()//Stops camera when player dies
    {

        if (!player)
            return;

        transform.position = player.position + offset;
    }

}//class







