# Automated test for TicTacToe to make sure that the AI cannot lose; either wins or ties.
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException
import unittest, time, re



def peek(list):
    item = list.pop()
    list.append(item)
    return item

class Tictactoe(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Chrome()
        self.driver.implicitly_wait(30)
        self.base_url = "https://www.google.com/"
        self.verificationErrors = []
        self.accept_next_alert = True
    
    def test_tictactoe(self):
        driver = self.driver
        driver.get("http://localhost:3000/")
        CurrMoves = [0]
        driver.find_element_by_xpath("//*[@id=0]").click()
        completed = False
        GameResults = []
        
        while(CurrMoves != [9]):
            lastmove = peek(CurrMoves) # 3
            # Game Over
            if(driver.find_element_by_xpath("//*[@id='value']").text != "Tic-Tac-Toe 3x3"):
                print("Game Winner:", driver.find_element_by_xpath("//*[@id='value']").text)
                if(driver.find_element_by_xpath("//*[@id='value']").text == "X"):
                    raise Exception("ERROR: AI LOST")
                # Click Reset
                if(not self.is_element_present(By.XPATH, "//*[@id='root']/div/div/p/button" )):
                    WebDriverWait(driver,10).until(expected_conditions.element_to_be_clickable((By.XPATH,"//*[@id='root']/div/div/p/button")))
                driver.find_element_by_xpath("//*[@id='root']/div/div/p/button").click()
                

                lastmove = CurrMoves.pop()
                while(len(CurrMoves) == 4 or lastmove == 8):
                    lastmove = CurrMoves.pop()
        
                i = 0
                while(i < len(CurrMoves)):
                    driver.find_element_by_xpath("//*[@id="+ str(CurrMoves[i]) +"]").click()
                    i += 1
                move = 1
                while(lastmove+move > 8 or driver.find_element_by_xpath("//*[@id="+ str(lastmove + move) +"]").get_attribute("color") != "w"):
                    move += 1
                    if(lastmove + move > 8):
                        if(len(CurrMoves) < 1):
                            lastmove += 1
                        else:
                            lastmove = CurrMoves.pop()
                        if(lastmove > 8):
                            completed = True
                            break
                        move = 0
                if(completed):
                    break
                driver.find_element_by_xpath("//*[@id="+ str(lastmove + move) +"]").click()
                CurrMoves.append(lastmove + move)

            # Active Game    
            else:
                # Find the next available move.
                move = 0
                while(driver.find_element_by_xpath("//*[@id="+ str(move) +"]").get_attribute("color") != "w"):
                    move += 1
                driver.find_element_by_xpath("//*[@id="+ str(move) +"]").click()
                CurrMoves.append(move)
        print("AI Did Not Lose. Pass Successfull")
            


    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True

    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)

if __name__ == "__main__":
    unittest.main()
