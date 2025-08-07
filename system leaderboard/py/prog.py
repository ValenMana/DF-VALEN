import tkinter as tk
from tkinter import simpledialog, messagebox, scrolledtext
import serial
import serial.tools.list_ports
import threading
import time

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Interfaz Arduino")
        self.serial_connection = None
        self.user_data = {}  # Diccionario para almacenar usuario y tag RFID

        # Pantalla principal
        self.main_frame = tk.Frame(root)
        self.main_frame.pack(pady=20, padx=20)

        self.connect_button = tk.Button(self.main_frame, text="Conectar", command=self.open_connection_window)
        self.connect_button.pack(pady=10)

        self.exit_button = tk.Button(self.main_frame, text="Salir", command=self.root.quit)
        self.exit_button.pack(pady=10)

    def open_connection_window(self):
        self.main_frame.pack_forget()

        self.connection_frame = tk.Frame(self.root)
        self.connection_frame.pack(pady=20, padx=20)

        tk.Label(self.connection_frame, text="Seleccione el puerto COM:").pack(pady=5)

        # Lista de puertos disponibles
        self.combobox = tk.Listbox(self.connection_frame, height=5, exportselection=0)
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.combobox.insert(tk.END, port.device)

        self.combobox.pack(pady=5)

        self.connect_serial_button = tk.Button(self.connection_frame, text="Conectar", command=self.connect_serial)
        self.connect_serial_button.pack(pady=5)

        self.back_button = tk.Button(self.connection_frame, text="Volver", command=self.go_back)
        self.back_button.pack(pady=5)

    def connect_serial(self):
        selected_port_index = self.combobox.curselection()
        if not selected_port_index:
            messagebox.showerror("Error", "Debe seleccionar un puerto COM")
            return

        selected_port = self.combobox.get(selected_port_index)

        try:
            self.serial_connection = serial.Serial(port=selected_port, baudrate=115200, timeout=1)
            messagebox.showinfo("Conexión exitosa", f"Conectado al puerto {selected_port}")
            self.open_turn_window()
        except serial.SerialException as e:
            messagebox.showerror("Error de conexión", f"No se pudo conectar al puerto {selected_port}\n{str(e)}")

    def open_turn_window(self):
        if self.connection_frame:
            self.connection_frame.pack_forget()

        self.turn_frame = tk.Frame(self.root)
        self.turn_frame.pack(pady=20, padx=20)

        tk.Label(self.turn_frame, text="Conexión establecida").pack(pady=5)

        self.new_turn_button = tk.Button(self.turn_frame, text="Nuevo Turno", command=self.new_user_screen)
        self.new_turn_button.pack(pady=10)

        self.exit_button = tk.Button(self.turn_frame, text="Salir", command=self.root.quit)
        self.exit_button.pack(pady=10)

    def new_user_screen(self):
        if self.serial_connection and self.serial_connection.is_open:
            try:
                self.serial_connection.write(b'-1\n')  # Enviar el comando -1 al Arduino como byte
            except serial.SerialException as e:
                messagebox.showerror("Error", f"No se pudo enviar el comando al Arduino\n{str(e)}")
                return

        self.turn_frame.pack_forget()

        self.user_screen = tk.Frame(self.root)
        self.user_screen.pack(pady=20, padx=20)

        tk.Label(self.user_screen, text="Nuevo Usuario").pack(pady=5)

        self.add_user_button = tk.Button(self.user_screen, text="Agregar Usuario", command=self.prompt_user_name)
        self.add_user_button.pack(pady=10)

        self.user_list_frame = tk.Frame(self.user_screen)
        self.user_list_frame.pack(pady=10)

        for user, tag in self.user_data.items():
            tk.Label(self.user_list_frame, text=f"{user}: {tag}").pack()

        self.back_button = tk.Button(self.user_screen, text="Volver", command=self.go_back_to_turn)
        self.back_button.pack(pady=10)

    def prompt_user_name(self):
        user_name = simpledialog.askstring("Nombre de Usuario", "Ingrese el nombre del usuario:")
        if not user_name:
            messagebox.showerror("Error", "Debe ingresar un nombre de usuario")
            return

        if self.serial_connection and self.serial_connection.is_open:
            try:
                self.serial_connection.write(b'1\n')  # Enviar el carácter '1' como byte
                self.serial_connection.write(b'1\n')  # Enviar el carácter '1' como byte
                self.serial_connection.write(b'1\n')  # Enviar el carácter '1' como byte
                self.serial_connection.write(b'1\n')  # Enviar el carácter '1' como byte

            except serial.SerialException as e:
                messagebox.showerror("Error", f"No se pudo enviar el comando al Arduino\n{str(e)}")
                return
        else:
            messagebox.showerror("Error", "No hay una conexión serial activa")
            return

        self.wait_for_rfid(user_name)

    def wait_for_rfid(self, user_name):
        if self.serial_connection:
            try:
                self.user_screen.pack_forget()

                self.rfid_screen = tk.Frame(self.root)
                self.rfid_screen.pack(pady=20, padx=20)

                tk.Label(self.rfid_screen, text="Esperando Tag RFID").pack(pady=5)

                # Terminal serial
                self.terminal = scrolledtext.ScrolledText(self.rfid_screen, width=50, height=10, state='disabled')
                self.terminal.pack(pady=10)

                threading.Thread(target=self.read_from_serial, daemon=True).start()

                tag_rfid = self.serial_connection.readline().decode('utf-8').strip()

                if tag_rfid:
                    self.user_data[user_name] = tag_rfid
                    messagebox.showinfo("Registro exitoso", f"Usuario: {user_name}\nTag RFID: {tag_rfid}")
                    self.refresh_user_list()
                    time.sleep(2)  # Espera 2 segundos
                    self.rfid_screen.pack_forget()
                    self.new_user_screen()
                else:
                    messagebox.showerror("Error", "No se recibió ningún tag RFID")
            except serial.SerialException as e:
                messagebox.showerror("Error", f"No se pudo leer el tag RFID\n{str(e)}")

    def read_from_serial(self):
        while True:
            if self.serial_connection:
                try:
                    line = self.serial_connection.readline().decode('utf-8').strip()
                    if line:
                        self.terminal.config(state='normal')
                        self.terminal.insert(tk.END, line + "\n")
                        self.terminal.see(tk.END)
                        self.terminal.config(state='disabled')
                except serial.SerialException:
                    break

    def refresh_user_list(self):
        for widget in self.user_list_frame.winfo_children():
            widget.destroy()

        for user, tag in self.user_data.items():
            tk.Label(self.user_list_frame, text=f"{user}: {tag}").pack()

    def go_back(self):
        self.connection_frame.pack_forget()
        self.main_frame.pack(pady=20, padx=20)

    def go_back_to_turn(self):
        self.user_screen.pack_forget()
        self.turn_frame.pack(pady=20, padx=20)

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
