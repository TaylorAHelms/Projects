using UnityEngine;

//Player Collision

public class PlayerCollision : MonoBehaviour
{

    public PlayerMovement movement;

    void OnCollisionEnter(Collision collisionInfo)//function runs when player collides with an obstacle
    {
        if(collisionInfo.collider.tag == "Obstacle")//On collision, player object is destroyed and game over sequence from game manager starts
        {
            Destroy(gameObject);
            FindObjectOfType<GameManager>().pauseBeforeEnding();
        }
    }


}//class
