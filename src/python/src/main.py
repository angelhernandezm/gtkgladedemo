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

class MyApp:
    def __init__(self):
        # Load the Glade file
        self.builder = Gtk.Builder()
        self.builder.add_from_file("Calculator.glade")

        # Connect signals to handlers
        self.builder.connect_signals(self)

        # Get the main window from the Glade file
        self.window = self.builder.get_object("frmMain")
        self.window.connect("destroy", Gtk.main_quit)
        self.window.show_all()

    def on_button_clicked(self, button):
        print("Button clicked!")

    def OnApplicationQuit(self, *args):
        Gtk.main_quit()

if __name__ == "__main__":
    try:
        DependencyChecker.ensure_package('PyGObject')
        import gi
        gi.require_version("Gtk", "3.0")
        from gi.repository import Gtk
        app = MyApp()
        Gtk.main()
    except Exception as e:
        print(e)


