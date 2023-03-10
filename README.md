# VR-Haptic-Gloves
This will be a fairly large scale experimental project used to explore innovative ways to interact with VR through the use of Haptic Gloves.

**Main Goal**: Develop an Experimental VR Glove Prototype with Positional Tracking, Finger Tracking and Force Feedback.

**Research Goal**: We want to prototype different approaches, this is not to build a working product. This is to experiment with different engineering methods
to produce an innovative way to interact with VR through hand gloves.

## 1. Identify methods to track hands in virtual reality

### 1.1 Should positional tracking come from the headset itself or should it come as part of a self built external system?

#### Hard Approach 

Through the use of external sensors, whether they are RGB cameras or lidar or Infrared, I should be able to build an external sensor system which functions 
as part of a positional tracking system. The tracking system will keep track of my hands and map them into 3d space. I can map these 3d coordinates to the driver API to
map my hands in steamVR. 

##### Difficulties
- How am I going to keep their positions in sync with my HMD? (Head Mounted Display)
The HMD display (quest 2) will not be aware of the positions of my hands in steamvr, hence it will likely end in a scenario where my hands are in a completely 
different coordinate location than where my HMD display is. The way to mitigate this is to also be able to determine where the headset position is, that way, I can map both 
the hands and HMD display simulataneously and keep them in sync, with my HMD display positioned in front of my hands. 

However, more difficulties arise as I will need to obtain individual IR trackers and IR transmitters to achieve this goal. 2 Trackers and 3 Individual Transmitters.
I would also need to ensure that the tracker-transmitter solution works and that I can feasibly map this to a 3d space and translate this to the OpenVR Driver. The estimated
time I'd require for work on this is 3 weeks to a 1 Month. 

##### Outcome
Does not seem worth it, despite the abundant learning opportunities I would come across. This could be a problem I expand upon when an experimental prototype with
satisfactory positional tracking has been delivered.

#### Medium Approach
The Quest 2 already has built in positional tracking for hands, I could just simply use this as a means for positional tracking without complication.

##### Difficulties
- With building the VR Glove, there are going to be a fair number of components and modifications where the hands may no longer be viably tracked by the quest 2's
hand tracking software. This will result in unsatisfactory positional hand tracking and it will likely result in flickering or my hands not even being picked up by the camera.

##### Outcome
Could work but this is highly coupled with how many modifications will be on the VR haptic glove. If anything, this is motivation to produce a minimalistic design
so the overall shape and look is still very much human and hand-like. Will need further experimentation to see if this is a viable option or not. 

#### Easy Approach
DIY a harness strap for the quest 2 controllers on my wrists. The controllers will be automatically picked up by the quest 2 headset and will result in 
near perfect precision for positional tracking. 

##### Difficulties
- Likely none, except for building a harness strap design. Another tradeoff is that it's a lousy solution with no interesting engineering being done. Plus it looks stupid.

##### Outcome
This is likely the solution if the Medium Approach does not work, but this will likely result in very smooth motion tracking, which is to be desired.

#### Resolution
Positional Tracking should generally come from the Headset itself, the external sensor system will result in overengineering and with most headsets becoming
standalone, we should try to adopt this method as well. 

#### Potential Solution
A custom camera component could be strapped onto the quest 2 headset itself, which functions as a positional tracker for the hands. 
I would need a custom battery unit and a bunch of other arduino components to feasibly put this on my quest 2 headset. We can try this once we've gotten a working prototype.

### 2, How will finger tracking be done? 

#### 2.1 What engineering contraption should be used to do finger tracking?

In Lucas VRTech's Video, he shows the usage of a spool. The contraption works by having strings attached from the backhand all the way down to the end of each finger.
Upon bending the finger, this will result in the pulling of string(s), which are tied to some rotary measurement device. This appears to be what is used to measure the 
relative rotation of each finger. 

#### 2.2 How Our Design Will Differ

In place of using strings and a rotary measurement device, we will instead just be using 5 individual servo motors. These can either be the micro servo or sub micro servo. I
will likely need to experiment with both to see what is the appropriate choice so I will be testing this design on an individual finger first. 

The motivation for our design is minimalism. The design for our finger tracking will be done purely through the servos, where the servos are built around an inspired
hinge mechanism found online. https://grabcad.com/library/servo-based-hinge-1 

The finger rotation will be translated into servo rotary motion through this design by encasing a finger in a ring. See diagram for further info.
This design will need initial experimentation to be turned into a proper working solution.

#### 2.3 Force Feedback

### 3. How do we transform the servo rotary encodings into finger rotations in steamVR?

Create an OpenVR Driver. (This will take a fair amount of research)

### 4. How will we determine when VR Character has picked up an object?

Will need to work out the algorithm for this. But it likely goes along the lines of reading in the input of the controller's fingers position when it has snapped into place
when picking up an object. Then when this position of fingers is detected, we lock the servo going beyond a certain rotation value.

#### 4.1 How will this turn into force feedback?



