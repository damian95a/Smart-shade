# Smart shade
The project was done during the Embedded systems classes. It consists of a model of a window with a shade, a microcontroller Open1768, and a stepper motor with a controller. There are 2 analog light sensors (one inner and one outer), that provide data for decisions to be made.

The main function is setting the shade position. If there is too dark it opens and closes when too bright. The system can also detect night defined as a given lightness or a set time of opening/closing.

![image](https://user-images.githubusercontent.com/79938517/218491137-4ee0229b-190c-4f2d-abd3-ca26686566b3.png)

The user interface consists of a display (ILI9325) and a 16-button keyboard. The screen displays the menu that allows you to change the configuration of the device. The currently chosen option is highlighted. While inserting values it is being real-time formatted. Cursor moves forward during setting the time.
User can set target brightness, sensor tolerance, current time, and motor limits. It is also possible to define night light level, schedule, and the way the night state is defined (only sensor/schedule, both sensor, and schedule, what happens first: sensor or schedule). The user interface has been written in Polish.

![image](https://user-images.githubusercontent.com/79938517/218491286-06ea6acd-30a4-4515-9646-cb38380f71f0.png)

![image](https://user-images.githubusercontent.com/79938517/218491419-2329f0f5-83ba-453e-b764-fc4589e70050.png)

The shade can work in two modes: manual - setting explicit position with the user interface, auto - setting shade position depending on light sensors and/or time.
