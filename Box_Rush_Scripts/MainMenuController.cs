using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MainMenuController : MonoBehaviour
{
    
    public void StartGame()
    {
        SceneManager.LoadScene("Level01");
    }

    public void HowToPlay()
    {
        SceneManager.LoadScene("HowToPlay");
    }





}//class
