using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//Collector
//Destroys obstacles once they move passed the player
//Score is increased by 10 for each obstacle that reaches the collector

public class Collector : MonoBehaviour
{

    public Text scoreText;
    public Text finalScoreText;
    public Transform Player;

    private int score = 0;

    private void OnTriggerEnter(Collider collision)//Obstacle is destroyed on collision with collector, if player is still alive: score +10
    {
        if (collision.CompareTag("Obstacle"))
        {
            Destroy(collision.gameObject);
            if(Player)
            {
                score += 10;
            }
            scoreText.text = score.ToString();
            finalScoreText.text = "Final Score: " + score.ToString();
        }
    }


}//class
