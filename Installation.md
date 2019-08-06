# Installation

## 0. Arduino IDE

To start programming your own things on Ringo phone you're going to need a software called **Arduino**. If you already have it installed on your computer, you can skip this step. 

Go to the adress: https://www.arduino.cc/en/Main/Software .

You will find this screen there:

![alt text](https://www.circuitmess.com/wp-content/uploads/Arduino.png)

Download the installation file according to the OS you own. If you have Windows 8.1 or later, you can also download Arduino IDE over Microsoft Store. That version of the software will be considered to be a completely different program than the regular version by your system so keep in mind that your libraries will not be in sync. Other than that, these two versions work the same.

Now when you have installed Arduino we can go to the next step!

## Installing Ringo Add-On in Arduino IDE

These instructions work on every OS since Arduino automatically downloads tools depending on detected system. So just follow these steps and you will be fine!

1. Open Arduino IDE

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic1.png)

**NOTE**: If this is your first time meeting Arduino IDE - get familliar, you're going to use it a lot.

2. Go to **File** -> **Preferences**

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic2.png)

3. Enter the following adress in the **'Additional board Manager URLs'** : **https://raw.githubusercontent.com/CircuitMess/MAKERphone/boardArduino/package_CircuitMess_Ringo_index.json**. Then click **OK**.

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic3.png)

**NOTE**: You can also click the little window button by the bar and enter it there, then click **OK**.     

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic5.png)

4. Go to **Tools** -> **Board** -> **Boards Manager...**

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic7.png)

5. Type **'Ringo'** into the bar on top of the pop-up window.
There will be one result found called **'Ringo by CircuitMess'**. Click on the **Install** button.

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic8.png)

**NOTE**: Main board package and a few tools will now download and install on your system. It all takes about **200 MB** so the time of download will depend on your internet connection. 

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic9.png)

6. When it all finished it will write out **'INSTALLED'** next to the version mark. Click on the **Close** button and move on.

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic10.png)

7. Now it's time to select the installed board. Again go to **Tools** -> **Board** and in the dropdown menu under category **'ESP32 Arduino'** find **'Ringo by CircuitMess'**. It should be somewhere at the bottom. Select the board.

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic11.png)

8. If you haven't already, connect your Ringo phone to the PC via micro-USB to USB cable. It will connect to one of the **COM** ports. Go to **Tools** and under **Board** section select **Port** and find the port on which Ringo is connected.  

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic13.png)

**NOTE**: If **Port** is greyed out it means the phone is not correctly connected. Change port and/or check your cable.

9. Now it's all set to write your first program! In the **void setup()** section write the following line:  
   
    ```cpp
    Serial.begin(115200);
     ``` 
   
   Under the **void loop()** section write something like this:   


    ```cpp
    Serial.print("My first Arduino program for MAKERphone...");
     ```

     Now click on the little **Upload** arrow in the upper left corner.

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic14.png)

10. After a few moments, the text should say **'Done uploading'**. Now only thing left to do is to check if everything is working properly. Click the magnifier icon in the upper right corner of the screen.

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic15-1.png)

11. On a newly opened window, from a drop down menu, select **'115200 baud'**. That is the speed that the software writes at in the **Serial Monitor**. If the text says the line that you wrote in the **print** function, then everything works!  

![alt text](https://www.circuitmess.com/wp-content/uploads/Pic16.png)


Congrats! You have successfully set up Ringo phone. Now let's get to business and start creating some cool apps and games!
