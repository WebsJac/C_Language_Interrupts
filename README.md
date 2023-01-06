C language code for the NIOS II processor that uses IO interrupts to perform a given function. These files have been created by Jack Webster using the template.c file provided by N. Manjikian.

---

<h4>16bitInportAndOutport.C</h4>
<p>Program uses a 0.5s timer to toggle the first 8 bits of the output port that control 8 LEDs.
The input port contains a 7 bits of data and a valid bit at bit 7</p>.
<p> The main program uses the valid bit to retrive the data and convert it to hex that is output to the last 8 bits of the output port that control the HEX DISPLAYS</p>

<h4>CompareCurrAndPrevData.c</h4>
<p>/*Program uses 2 timers, one to cycle LEDs and one to tell main program to compare past and present inport values.</p>
<p>Prints >, = or < based on comparison and updates a running count, sum and average of all inport values*/</p>

<h4>HeaterApplication.c</h4>
<p></p>

