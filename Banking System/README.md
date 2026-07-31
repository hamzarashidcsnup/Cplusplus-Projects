V1:
Original Version of the code.
Uses vector to store data of user and transactions.
Features include account creation, deposit, withdraw and transfer money.

V2:
Modified the original version of code.
**Changes Made:**
Data is now permanently stored in a .txt file as well as in vector array.
Added a storinguser function and a storingTransactionHistory function to write the data into a file.
Added two functions to load the data from the files when the program starts.
Emptied out the constructor in Customer class and made a new function to register a new user.
Fixed bugs, bugs included; negative amount entered to withdraw, transfer or deposit; transfer money into your own account(sender and receiver account numbers could be same);

V3:
Revamped the entire code.
**Changes made:**
Account number and password were added that makes user login to their account and they can withdraw, deposit or transfer without having to enter password every time they wanted to perform an action.
Code was entirely rehauled focusing on encapsulation, adding getters and setters and making public class variables private.
A dedicated class method was made entirely to track the transaction history.
Improved readability.
Menu is changed, letting user login or register at the start.
