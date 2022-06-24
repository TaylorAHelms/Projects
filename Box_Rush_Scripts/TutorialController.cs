using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

//Tutorial Controler

public class TutorialController : MonoBehaviour
{
    public void BackToMenu()//navigates to Main Menu on button press
    {
        SceneManager.LoadScene("MainMenu");
    }
}//class
