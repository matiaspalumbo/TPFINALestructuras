import pyautogui

def calcular():
  f = open("operaciones.txt", "r")
  lineas = f.readlines()
  for i in range(len(lineas)):
    pyautogui.write(lineas[i])


calcular()
