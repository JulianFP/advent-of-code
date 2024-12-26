use std::env;
use std::fs::File;
use std::io::Read;
use regex::Regex;
use std::collections::HashSet;

fn parse_rules(input_string: &str) -> HashSet<(u32, u32)> {
    let re = Regex::new(r"(\d+)\|(\d+)").unwrap();
    let mut return_val = HashSet::new();
    for cap in re.captures_iter(input_string) {
        let (_, [first, second]) = cap.extract();
        return_val.insert((first.parse::<u32>().unwrap(), second.parse::<u32>().unwrap()));
    }

    return_val
}

fn parse_updates(input_string: &str) -> Vec<Vec<u32>> {
    let mut return_val = Vec::new();
    for line in input_string.lines() {
        return_val.push(line.split(",").map(|page| {
            page.parse::<u32>().unwrap()
        }).collect());
    }
    return_val
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide a filename as a cli argument");
    }
    let mut file = File::open(&args[1]).expect("Provided file could not be opened");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Provided file could not be read");

    let (rules, updates) = contents.split_once("\n\n").expect("File should contain both rules and updates separated by two newlines");
    let rules_set = parse_rules(rules);
    let updates_vec = parse_updates(updates);
    let mut updates_incorrect = Vec::new();

    let mut result = 0;
    for update in updates_vec {
        let mut is_correct = true;
        'outer: for (i, page) in update.iter().enumerate() {
            for future_page in update[i+1..].iter() {
                if rules_set.contains(&(*future_page, *page)) {
                    is_correct = false;
                    break 'outer;
                }
            }
        }
        if is_correct {
            result += update[update.len() / 2];
        }
        else {
            updates_incorrect.push(update);
        }
    }
    println!("The solution of the first part is {result}");

    //fix incorrect updates
    //this is super inefficient...
    result = 0;
    for mut update in updates_incorrect {
        'curr_update_loop: loop {
            'outer: for i in 0..update.len() {
                for j in i+1..update.len() {
                    if rules_set.contains(&(update[j], update[i])) {
                        let removed_page = update.remove(i);
                        update.insert(j, removed_page);
                        break 'outer;
                    }
                }
                if i == update.len()-1 {
                    result += update[update.len() / 2];
                    break 'curr_update_loop;
                }
            }
        }
    }
    println!("The solution of the second part is {result}");
}
