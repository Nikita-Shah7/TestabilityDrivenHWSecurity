#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

class Gate
{
    string type;
    string name;
    vector<string> inputs;
    vector<string> outputs;

public:
    Gate();
    friend void parse_verilog_file(string);
    friend void generate_text_file(string);
};

Gate::Gate(void)
{
    name = "";
    type = "";
}

vector<Gate> gate_list;

void parse_verilog_file(string filename)
{
    regex inputRegex(R"(input\s+([a-zA-Z0-9_,\s]+);)");
    regex outputRegex(R"(output\s+([a-zA-Z0-9_,\s]+);)");
    regex wireRegex(R"(wire\s+([a-zA-Z0-9_,\s]+);)");
    regex andRegex(R"(and\s+([()a-zA-Z0-9_,\s]+);)");
    regex orRegex(R"(or\s+([()a-zA-Z0-9_,\s]+);)");
    regex nandRegex(R"(nand\s+([()a-zA-Z0-9_,\s]+);)");
    regex norRegex(R"(nor\s+([()a-zA-Z0-9_,\s]+);)");
    regex xorRegex(R"(xor\s+([()a-zA-Z0-9_,\s]+);)");
    regex xnorRegex(R"(xnor\s+([()a-zA-Z0-9_,\s]+);)");
    regex notRegex(R"(not\s+([()a-zA-Z0-9_,\s]+);)");
    regex bufRegex(R"(buf\s+([()a-zA-Z0-9_,\s]+);)");
    regex dffRegex(R"(dff\s+([()a-zA-Z0-9_,\s]+);)");
    regex fanoutRegex(R"(buf\s+([()a-zA-Z0-9_,\s]+);)");
    regex ignorePattern0("^\\s*module\\s+dff\\s*\\(.*\\);\\s*$");
    regex ignorePattern1("not P3 \\([^,]+,[^)]+\\);");
    regex ignorePattern2("not P5 \\([^,]+,[^)]+\\);");
    regex ignorePattern3("not P1 \\([^,]+,[^)]+\\);");

    ifstream file(filename);
    if (!file)
    {
        cerr << "ERROR MESSAGE:: Enable to open the file!!";
        return;
    }
    string line = "";
    int fanout_cnt = 0;
    while (getline(file, line))
    {
        // cout << line << endl;
        smatch match;
        Gate gate;
        if (regex_search(line, match, ignorePattern0) || regex_search(line, match, ignorePattern1) || regex_search(line, match, ignorePattern2) || regex_search(line, match, ignorePattern3))
        {
            continue;
        }
        else if (regex_search(line, match, nandRegex))
        {
            gate.type = "nand";
        }
        else if (regex_search(line, match, andRegex))
        {
            gate.type = "and";
        }
        else if (regex_search(line, match, xnorRegex))
        {
            gate.type = "xnor";
        }
        else if (regex_search(line, match, xorRegex))
        {
            gate.type = "xor";
        }
        else if (regex_search(line, match, norRegex))
        {
            gate.type = "nor";
        }
        else if (regex_search(line, match, orRegex))
        {
            gate.type = "or";
        }
        else if (regex_search(line, match, notRegex))
        {
            gate.type = "not";
        }
        else if (regex_search(line, match, bufRegex))
        {
            gate.type = "buf";
        }
        else if (regex_search(line, match, dffRegex))
        {
            gate.type = "dff";
        }
        else if (regex_search(line, match, fanoutRegex))
        {
            gate.type = "fanout";
        }

        if (gate.type == "")
            continue;
        else if (gate.type == "fanout")
        {
            string wires = match[1].str();                                     // "G1 (a5,a1,a2)"
            wires.erase(remove(wires.begin(), wires.end(), ' '), wires.end()); // remove extra spaces from between ("G1(a5,a1,a2)")
            stringstream line_stream(wires);

            // gate gate_name
            string g_name = "F" + to_string(++fanout_cnt);
            // getline(line_stream, g_name, '(');
            gate.name = g_name;

            // get gate_input
            string input = "";
            getline(line_stream, input, ',');
            gate.inputs.push_back(input);

            // get gate_outputs
            string output = "";
            while (getline(line_stream, output, ','))
                gate.outputs.push_back(output);
            string last_output = gate.outputs.back();
            last_output = last_output.substr(0, last_output.size() - 1);
            gate.outputs[gate.outputs.size() - 1] = last_output;
        }
        else if (gate.type == "dff")
        {
            string gate_plus_wires = match[1].str();                                                                   // "G1 (a5,a1,a2)"
            gate_plus_wires.erase(remove(gate_plus_wires.begin(), gate_plus_wires.end(), ' '), gate_plus_wires.end()); // remove extra spaces from between ("G1(a5,a1,a2)")
            stringstream line_stream(gate_plus_wires);

            // get gate_name
            string g_name = "";
            getline(line_stream, g_name, '(');
            gate.name = g_name;

            // get dff_clock
            string clock = "";
            getline(line_stream, clock, ',');
            gate.inputs.push_back(clock);

            // get dff_output
            string output = "";
            getline(line_stream, output, ',');
            gate.outputs.push_back(output);

            // get dff_input
            string input = "";
            getline(line_stream, input, ',');
            gate.inputs.push_back(input);
            string last_input = gate.inputs.back();
            last_input = last_input.substr(0, last_input.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = last_input;
        }
        else
        {
            string gate_plus_wires = match[1].str();                                                                   // "G1 (a5,a1,a2)"
            gate_plus_wires.erase(remove(gate_plus_wires.begin(), gate_plus_wires.end(), ' '), gate_plus_wires.end()); // remove extra spaces from between ("G1(a5,a1,a2)")
            stringstream line_stream(gate_plus_wires);

            // gate gate_name
            string g_name = "";
            getline(line_stream, g_name, '(');
            gate.name = g_name;

            // get gate_output
            string output = "";
            getline(line_stream, output, ',');
            gate.outputs.push_back(output);

            // get gate_inputs
            string input = "";
            while (getline(line_stream, input, ','))
                gate.inputs.push_back(input);
            string last_input = gate.inputs.back();
            last_input = last_input.substr(0, last_input.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = last_input;
        }

        gate_list.push_back(gate);
    }
}

void generate_text_file(string filename)
{
    ofstream output_file(filename);
    if (!output_file)
    {
        cerr << "ERROR MESSAGE:: Enable to open the file!!";
        return;
    }
    for (size_t i = 0; i < gate_list.size(); ++i)
    {

        auto gate = gate_list[i];
        string type = gate.type;
        transform(type.begin(), type.end(), type.begin(), ::toupper);
        output_file << type << " ";
        string name = gate.name;
        output_file << name << " ";
        output_file << gate.outputs[0] << " ";
        for (auto input : gate.inputs)
        {
            output_file << input << " ";
        }
        if (i != gate_list.size() - 1)
            output_file << endl;
    }
    output_file.close();
}

void display(string filename)
{
    cout << "\nOUTPUT_FILE::\n";
    ifstream file(filename);
    if (!file)
    {
        cerr << "ERROR MESSAGE:: Enable to open the file!!";
        return;
    }
    string line = "";
    while (getline(file, line))
        cout << line << endl;

    file.close();
}

int main()
{
    string input_folder = "../verilog_files/";
    string output_folder = "../input_text_files/";

    // DIR *dir;
    // struct dirent *ent;
    // if ((dir = opendir(input_folder.c_str())) != NULL)
    // {
    //     while ((ent = readdir(dir)) != NULL)
    //     {
    //         string file_name = ent->d_name;
    //         if (file_name == "." || file_name == "..")
    //             continue;
    //         file_name = file_name.substr(0, file_name.find_last_of(file_name));
    //         string input_file = input_folder + file_name + "v";
    //         string output_file = output_folder + file_name + "txt";
    //         // cout<<input_file<<endl<<output_file<<endl<<endl;
    //         parse_verilog_file(input_file);
    //         generate_text_file(output_file);
    //         // display(output_file);
    //         gate_list.clear();
    //     }
    //     closedir(dir);
    // }
    // else
    // {
    //     // Could not open directory
    //     cerr << "Error opening directory " << input_folder << endl;
    //     return EXIT_FAILURE;
    // }

    string file_name = "";
    cout << "Enter Input Text File: ";
    cin >> file_name;
    // file_name = "a5";
    string input_file = input_folder + file_name + ".v";
    string output_file = output_folder + file_name + ".txt";

    parse_verilog_file(input_file);
    generate_text_file(output_file);
    // display(output_file);

    return 0;
}