from tkinter import *

root = Tk()
root.geometry("200x150")

frame = Frame(root)
frame.pack()

## Window and Frame Tutorial

# leftFrame = Frame(root, bg= "blue", bd= 3)
# leftFrame.pack(side=LEFT)

# rightFrame = Frame(root, bg="red", bd = 3)
# rightFrame.pack(side=RIGHT)

# label = Label(frame, text="Hello World")
# label.pack()

# button1 = Button(leftFrame, text = "Button1")
# button1.pack(padx = 3, pady = 3)
# button2 = Button(rightFrame, text = "Button2")
# button2.pack(padx = 3, pady = 3)
# button3 = Button(leftFrame, text = "Button3")
# button3.pack(padx = 3, pady = 3)
 
# root.title("Test")
# root.mainloop()

## Button Tutorial

# def set():
#     print("Hello World")

# button = Button(frame, text = "Press Me!", command= set, fg="red", font="Verdana 14 underline", bd= 2, bg= "light blue", relief= "groove", highlightcolor= "blue")
# button.pack(pady=15)

# root.mainloop()

## Entry Tutorial

# def retrieve():
#     print(my_entry.get())
#     print(my_entry2.get())

# my_entry = Entry(frame, width= 20)
# my_entry.insert(0, 'Username')
# my_entry.pack(padx= 5, pady= 5)

# my_entry2 = Entry(frame, width= 20)
# my_entry2.insert(0, 'password')
# my_entry2.pack(padx= 5, pady= 5)

# button = Button(frame, text = "Submit", command= retrieve)
# button.pack(padx= 5, pady= 5)

# root.mainloop()

## Check Button Tutorial

# def retrieve():
#     print(Var1.get())
#     print(Var2.get())
 
# Var1 = IntVar()
# Var2 = IntVar()
 
# ChkBttn = Checkbutton(frame, text = "Option1", variable = Var1)
# ChkBttn.pack(padx = 5, pady = 5)
 
# ChkBttn2 = Checkbutton(frame, text = "Option2", variable = Var2)
# ChkBttn2.pack(padx = 5, pady = 5)
 
# Button = Button(frame, text = "Submit", command = retrieve)
# Button.pack(padx = 5, pady = 5)
 
# root.mainloop()

# Var1 = IntVar()
# Var2 = IntVar()
 
# ChkBttn = Checkbutton(frame, text = "Burger", variable = Var1, command = retrieve)
# ChkBttn.pack(padx = 5, pady = 5)
 
# ChkBttn2 = Checkbutton(frame, text = "Pizza", variable = Var2, command = retrieve)
# ChkBttn2.pack(padx = 5, pady = 5)
 
# root.mainloop()

## Label Tutorial

def set():
    var.set("Good-Bye Cruel World")
 
var = StringVar()
var.set("Hello World")
 
label = Label(frame, textvariable = var )
label.pack()
button = Button(frame, text = "set", command = set)
button.pack()
 
root.mainloop()

