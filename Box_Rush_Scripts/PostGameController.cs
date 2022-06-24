using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

//Post-Game Controller

public class PostGameController : MonoBehaviour
{
    public void Retry()//restarts the game on button press
    {
        SceneManager.LoadScene("Level01");
    }

    public void BackToMenu()//returns to main menu on button press
    {
        SceneManager.LoadScene("MainMenu");
    }


}//class
