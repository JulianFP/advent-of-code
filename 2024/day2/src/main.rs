use std::env;
use std::fs::File;
use std::io::Read;

fn report_is_safe(report: &Vec<i32>) -> bool {
    //first iteration decides whether ascending or descending
    let mut acc = false;
    if report[1] == report[0] || (report[1] - report[0]).abs() > 3 {
        return false;
    }
    else if report[1] > report[0] {
        acc = true;
    }

    //now iterate
    let mut last_level = report[1];
    for &level in report.iter().skip(2) {
        let diff = level - last_level;
        if acc {
            if diff < 1 || diff > 3 {
                return false;
            }
        }
        else {
            if diff < -3 || diff > -1 {
                return false;
            }
        }

        last_level = level;
    }

    return true;
}

fn main() {
    //parsing
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide a filename as a cli argument");
    }
    let mut file = File::open(&args[1]).expect("Provided file could not be opened");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Provided file coult not be read");

    let mut reports: Vec<Vec<i32>> = Vec::new();
    for line in contents.lines() {
        let mut current_report: Vec<i32> = Vec::new();

        let line_splitted: Vec<&str> = line.split(" ").collect();
        if contents.len() < 2 {
            panic!("Every report is supposed to have at least 2 levels");
        }

        for level in line_splitted {
            current_report.push(level.parse::<i32>().expect("Provided file has wrong format"));
        }
        reports.push(current_report);
    }


    //first part
    let mut report_safe_status: Vec<bool> = Vec::new(); let mut result_first_part = 0;
    for report in &reports[..] {
        let is_safe = report_is_safe(&report);
        report_safe_status.push(is_safe);
        if is_safe {
            result_first_part += 1;
        }
    }
    println!("The solution for the first part is {result_first_part}");

    //second part
    //I'm lazy so I just try to remove every element once to see if it works. This is ugly as hell
    let mut report_safe_status: Vec<bool> = Vec::new(); let mut result_first_part = 0;
    for report in &reports[..] {
        let mut is_safe = report_is_safe(&report);
        if !is_safe {
            for (i, _) in report.iter().enumerate() {
                let mut cloned_report = report.clone();
                cloned_report.remove(i);
                if report_is_safe(&cloned_report) {
                    is_safe = true;
                    break;
                }
            }
        }
        report_safe_status.push(is_safe);
        if is_safe {
            result_first_part += 1;
        }
    }
    println!("The solution for the second part is {result_first_part}");

}
