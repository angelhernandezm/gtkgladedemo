use gtk::prelude::*;
use gtk::{Application, ApplicationWindow, Button, Builder, Entry};
use std::cell::RefCell;
use std::rc::Rc;

fn main() {
    let app = Application::builder()
        .application_id("com.angel.calculator")
        .build();

    app.connect_activate(|app| {
        let ui_src = include_str!("../ui/calculator.glade");
        let builder = Builder::from_string(ui_src);

        let window: ApplicationWindow = builder.object("frmMain").unwrap();
        window.set_application(Some(app));

        // Get the display entry
        let entry: Entry = builder.object("entry_display").unwrap();

        // Shared state for current expression
        let expression = Rc::new(RefCell::new(String::new()));

        // Helper to wire a button by id
        let connect_button = |id: &str, expr: Rc<RefCell<String>>, entry: Entry| {
            let entry_clone = entry.clone();
            let button: Button = builder.object(id).unwrap();
            let label = button.label().unwrap_or_else(|| "".into());

            button.connect_clicked(move |_| {
                let mut exp = expr.borrow_mut();
                exp.push_str(&label);
                entry_clone.set_text(&exp);
            });
        };

        // Wire numeric buttons
        for id in &["btn_0","btn_1","btn_2","btn_3","btn_4","btn_5","btn_6","btn_7","btn_8","btn_9"] {
            connect_button(id, expression.clone(), entry.clone());
        }

        // Wire operator buttons
        for id in &["btn_add","btn_subtract","btn_multiply","btn_divide"] {
            connect_button(id, expression.clone(), entry.clone());
        }

        // Clear button
        {
            let button: Button = builder.object("btn_clear").unwrap();
            let entry_clone = entry.clone();
            let expr = expression.clone();
            button.connect_clicked(move |_| {
                expr.borrow_mut().clear();
                entry_clone.set_text("");
            });
        }

        // Equals button
        {
            let entry_clone = entry.clone();
            let button: Button = builder.object("btn_equals").unwrap();
            let expr = expression.clone();
            button.connect_clicked(move |_| {
                let exp = expr.borrow().clone();
                let result = evaluate_expression(&exp);
                entry_clone.set_text(&result);
                expr.borrow_mut().clear();
            });
        }
        window.show_all();
    });

    app.run();
}

// Expression evaluator
fn evaluate_expression(exp: &str) -> String {
    let expr = exp.replace("×", "*").replace("÷", "/").replace("−", "-");

    match meval::eval_str(&expr) {
        Ok(val) => val.to_string(),
        Err(_) => "Error".to_string(),
    }
}
