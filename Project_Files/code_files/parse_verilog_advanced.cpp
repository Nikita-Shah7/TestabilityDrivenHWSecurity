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
    // regex norRegex("nor2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex nandRegex("nnd\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex andRegex("and2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex orRegex("or2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex xorRegex("xor2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex xnorRegex("xnor2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    regex nandRegex("^nnd");
    regex norRegex("^nor");
    regex andRegex("^and");
    regex orRegex("^or");
    regex xorRegex("^xor");
    regex xnorRegex("^xnr");
    regex mux21Regex("^mxi21");
    regex buf1Regex("^hi");
    regex buf2Regex("^nb");
    regex buf3Regex("^i");
    regex buf4Regex("^ib");

    ifstream file(filename);
    string line = "";
    while (getline(file, line))
    {
        // cout << line << endl;
        smatch match;
        Gate gate;

        if (regex_search(line, match, nandRegex))
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
        else if (regex_search(line, match, mux21Regex))
        {
            gate.type = "mux2_1";
        }
        else if (regex_search(line, match, buf1Regex) || regex_search(line, match, buf2Regex) || regex_search(line, match, buf3Regex) || regex_search(line, match, buf4Regex))
        {
            gate.type = "buf";
        }

        if (gate.type == "")
            continue;
        else if (gate.type == "dff")
        {
            ;
        }
        else if (gate.type == "buf")
        {
            regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
            smatch instance_name_match;
            // string name = "";
            if (regex_search(line, instance_name_match, instance_name_regex))
                gate.name = instance_name_match[1].str();

            if (gate.name == "")
                continue;

            regex output_regex("\\.Q\\(([^\\)]*)\\)");
            smatch output_match;
            // string output = "";
            if (regex_search(line, output_match, output_regex))
                gate.outputs.push_back(output_match[1].str());

            if (gate.outputs.size() == 0)
                continue;

            regex input_regex("\\.DIN\\(([^\\)]*)\\)");
            smatch input_match;
            if (regex_search(line, input_match, input_regex))
                gate.inputs.push_back(input_match[1].str());

            if (gate.inputs.size() == 0)
                continue;
        }
        else if (gate.type == "mux2_1")
        {
            regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
            smatch instance_name_match;
            // string name = "";
            if (regex_search(line, instance_name_match, instance_name_regex))
                gate.name = instance_name_match[1].str();

            if (gate.name == "")
                continue;

            regex output_regex("\\.Q\\(([^\\)]*)\\)");
            smatch output_match;
            // string output = "";
            if (regex_search(line, output_match, output_regex))
                gate.outputs.push_back(output_match[1].str());

            if (gate.outputs.size() == 0)
                continue;

            regex select_line_regex("\\.SIN\\(([^\\)]*)\\)");
            smatch select_line_match;
            if (regex_search(line, select_line_match, select_line_regex))
                gate.inputs.push_back(select_line_match[1].str());

            regex input_regex("\\.DIN\\d+\\(([^\\)]*)\\)");
            sregex_iterator iter(line.begin(), line.end(), input_regex);
            sregex_iterator end;
            while (iter != end)
            {
                smatch match = *iter;
                if (match[1].matched)
                    gate.inputs.push_back(match[1].str());
                if (match[2].matched)
                    gate.inputs.push_back(match[2].str());
                ++iter;
            }

            if (gate.inputs.size() == 0)
                continue;
        }
        else
        {
            regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
            smatch instance_name_match;
            // string name = "";
            if (regex_search(line, instance_name_match, instance_name_regex))
                gate.name = instance_name_match[1].str();

            if (gate.name == "")
                continue;

            regex output_regex("\\.Q\\(([^\\)]*)\\)");
            smatch output_match;
            // string output = "";
            if (regex_search(line, output_match, output_regex))
                gate.outputs.push_back(output_match[1].str());

            if (gate.outputs.size() == 0)
                continue;

            regex input_regex("\\.DIN\\d+\\(([^\\)]*)\\)");
            sregex_iterator iter(line.begin(), line.end(), input_regex);
            sregex_iterator end;
            while (iter != end)
            {
                smatch match = *iter;
                if (match[1].matched)
                    gate.inputs.push_back(match[1].str());
                if (match[2].matched)
                    gate.inputs.push_back(match[2].str());
                ++iter;
            }

            if (gate.inputs.size() == 0)
                continue;
        }

        gate_list.push_back(gate);

        // if(regex_search(line,match,ignorePattern))
        // {
        //     continue;
        // }
        // else if (regex_search(line, match, dffRegex))
        // {
        //     gate.type = "dff";
        // }
    }
}

void generate_text_file(string filename)
{
    ofstream output_file(filename);
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
    string line = "";
    while (getline(file, line))
        cout << line << endl;

    file.close();
}

int main()
{
    string input_folder = "../verilog_files/";
    string output_folder = "../example_input_files/";

    string file_name = "dump1";
    // string file_name = "c2670";
    string input_file = input_folder + file_name + ".v";
    string output_file = output_folder + file_name + ".txt";

    parse_verilog_file(input_file);
    generate_text_file(output_file);
    // display(output_file);

    return 0;
}