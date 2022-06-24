using UnityEngine;
using UnityEngine.SceneManagement;

//Game Manager

public class GameManager : MonoBehaviour
{

    public GameObject gameOverUI;
    public GameObject Score;


    public void pauseBeforeEnding()//causes a 2 second delay before loading the game over screen
    {
        Score.SetActive(false);
        Invoke("GameOver", 2);
    }

    public void GameOver()//loads the game over screen
    {
        gameOverUI.SetActive(true);
    }




}//class
