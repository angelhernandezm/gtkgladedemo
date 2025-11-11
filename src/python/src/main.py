import sys
import subprocess

class DependencyChecker:
    @staticmethod
    def ensure_package(package_name):
        try:
            __import__(package_name)
        except ImportError:
            print(f"Installing missing package: {package_name}...")
            subprocess.check_call([sys.executable, "-m", "pip", "install", package_name])
            print(f"Package '{package_name}' was installed successfully.")

DependencyChecker.ensure_package('PyGObject')
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk

class Calculator(Gtk.Application):
    def __init__(self):
        super().__init__(application_id="com.angel.calculator")

    def do_activate(self):
        builder = Gtk.Builder()
        builder.add_from_file("ui/calculator.glade")

        window = builder.get_object("frmMain")
        window.set_application(self)

        entry = builder.get_object("entry_display")
        self.expression = ""

        # Wire numeric buttons
        for i in range(10):
            btn = builder.get_object(f"btn_{i}")
            btn.connect("clicked", self.on_button_clicked, entry)

        # Wire operator buttons
        for op in ["add", "subtract", "multiply", "divide"]:
            btn = builder.get_object(f"btn_{op}")
            btn.connect("clicked", self.on_button_clicked, entry)

        # Clear button
        builder.get_object("btn_clear").connect("clicked", self.on_clear_clicked, entry)

        # Equals button
        builder.get_object("btn_equals").connect("clicked", self.on_equals_clicked, entry)

        window.show_all()

    def on_button_clicked(self, button, entry):
        label = button.get_label()
        self.expression += label
        entry.set_text(self.expression)

    def on_clear_clicked(self, button, entry):
        self.expression = ""
        entry.set_text("")

    def on_equals_clicked(self, button, entry):
        try:
            expr = (self.expression
                    .replace("×", "*")
                    .replace("÷", "/")
                    .replace("−", "-"))
            result = eval(expr)
            entry.set_text(str(result))
            self.expression = ""
        except Exception:
            entry.set_text("Error")
            self.expression = ""

if __name__ == "__main__":
    try:
        app = Calculator()
        app.run([])
    except Exception as e:
        print(e)


