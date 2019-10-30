# Installation

## 0. Arduino IDE

To start programming your own things on Ringo phone you're going to need a software called **Arduino**. If you already have it installed on your computer, you can skip this step. 

Go to the adress: https://www.arduino.cc/en/Main/Software .

You will find this screen there:

<img src="https://www.circuitmess.com/wp-content/uploads/Arduino.png" alt="drawing" width="700"/>

Download the installation file according to the OS you own. If you have Windows 8.1 or later, you can also download Arduino IDE over Microsoft Store. That version of the software will be considered to be a completely different program than the regular version by your system so keep in mind that your libraries will not be in sync. Other than that, these two versions work the same.

Now when you have installed Arduino we can go to the next step.

## Installing Ringo Add-On in Arduino IDE

These instructions work on every OS since Arduino automatically downloads tools depending on detected system.  
So just follow these steps and you will be fine!

1. Open Arduino IDE

<img src="https://www.circuitmess.com/wp-content/uploads/Pic1.png" alt="drawing" width="700"/>

**NOTE**: If this is your first time meeting Arduino IDE - get familliar, you're going to use it a lot.

2. Go to **File** -> **Preferences**

<img src="https://www.circuitmess.com/wp-content/uploads/Pic2.png" alt="drawing" width="700"/>

3. Enter the following adress in the **'Additional board Manager URLs'** : **https://raw.githubusercontent.com/CircuitMess/MAKERphone/boardArduino/package_CircuitMess_Ringo_index.json**. Then click **OK**.


<img src="https://www.circuitmess.com/wp-content/uploads/Pic3.png" alt="drawing" width="700"/>

**NOTE**: You can also click the little window button by the bar and enter it there, then click **OK**.     

<img src="https://www.circuitmess.com/wp-content/uploads/Pic5.png" alt="drawing" width="700"/>

4. Go to **Tools** -> **Board** -> **Boards Manager...**

<img src="https://www.circuitmess.com/wp-content/uploads/Pic7.png" alt="drawing" width="700"/>

5. Type **'Ringo'** into the bar on top of the pop-up window.  
There will be one result found called **'Ringo by CircuitMess'**.   
Click on the **Install** button.

<img src="https://www.circuitmess.com/wp-content/uploads/Pic8.png" alt="drawing" width="700"/>

**NOTE**: Main board package and a few tools will now download and install on your system.  
It all takes about **200 MB** so the time of download will depend on your internet connection. 

<img src="https://www.circuitmess.com/wp-content/uploads/Pic9.png" alt="drawing" width="700"/>

6. When it's all finished it will write out **'INSTALLED'** next to the version mark.  
 Click on the **Close** button and move on.
 
 **NOTE**: The board file will be updated from time to time so make sure to check the version from time to time. When there is an option **Update**, please click it in order to stay in touch with the latest firmware.

<img src="https://www.circuitmess.com/wp-content/uploads/Pic10.png" alt="drawing" width="700"/>

7. Now it's time to select the installed board. Again go to **Tools** -> **Board** and in  
  the dropdown menu under category **'ESP32 Arduino'** find **'Ringo by CircuitMess'**.  
   It should be somewhere at the bottom. Select the board.

<img src="https://www.circuitmess.com/wp-content/uploads/Pic11.png" alt="drawing" width="700"/>

8. If you haven't already, connect your Ringo phone to the PC via micro-USB to USB cable.   
It will connect to one of the **COM** ports.  
 Go to **Tools** and under **Board** section select **Port** and find the port on which Ringo is connected.  

<img src="https://www.circuitmess.com/wp-content/uploads/Pic13.png" alt="drawing" width="700"/>

**NOTE**: If **Port** is greyed out it means the phone is not correctly connected.  
Change USB port and/or check your cable.

9. Now it's all set up to write your first program! In the **void setup()** section write the following line:  
   
    ```cpp
    Serial.begin(115200);
     ``` 
   
   Under the **void loop()** section write something like this:   


    ```cpp
    Serial.print("My first Arduino program for CircuitMess Ringo! ");
     ```

     Now click on the little **Upload** arrow in the upper left corner.

<img src="https://www.circuitmess.com/wp-content/uploads/Pic14-1.png" alt="drawing" width="700"/>

10. After a few moments, the text should say **'Done uploading'**.   
Now only thing left to do is to check if everything is working properly.  
 Click the magnifier icon in the upper right corner of the screen.

<img src="https://www.circuitmess.com/wp-content/uploads/Pic15-2.png" alt="drawing" width="700"/>

11. On a newly opened window, from a drop down menu, select **'115200 baud'**.  
 That is the speed that the software writes at in the **Serial Monitor**.   
 If the text says the line that you wrote in the **print** function, then everything works!  

<img src="https://www.circuitmess.com/wp-content/uploads/Pic16-1.png" alt="drawing" width="700"/>

12. Whenever you want to re-upload 'Default Ringo Software' you have to go to **Tools** -> **Programmer** and select
 **"Ringo Firmware"**.

<img src="https://www.circuitmess.com/wp-content/uploads/Snip1-R2.png" alt="drawing" width="700"/>

 Now go to **Tools** -> **Burn Bootloader** and you will have the default firmware on your Ringo as soon as 
 the upload is finished.

<img src="https://www.circuitmess.com/wp-content/uploads/Snip2-R2.png" alt="drawing" width="700"/>

13. There is also a Ringo library in Arduino Library Manager that allows you to use all the important functions and libraries from the Ringo firmware when writing your own programs. Go to **Sketch**-> **Include Library** -> **Manage Libraries...**. Enter the keyword **'Ringo'** in the search bar and click the **'Install'** button to download and install the library.

![img](https://github.com/CircuitMess/CircuitMess-Ringo/blob/musicPlayer-volume-txtInput/Arduino_LMB.png)
 

**Congrats! You have successfully set up Ringo phone.  
 Now let's get to business and start creating some cool apps and games!**
