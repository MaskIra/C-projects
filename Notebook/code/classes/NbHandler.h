#pragma once
#include "Notebook.h"
#include "Filter.h"
#include <time.h>
#include <fstream>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;

// ����� ��� ����� ������� ������ � Notebook
class NbHandler {
private:
	Notebook* nb; // �� ������, ����� �������� �����������
	const string delimiter = " | "; // ����������� ����� ������� ��� ������ � ������ �� �����
public:
	NbHandler(Notebook* nb) : nb(nb) { };

	void display_notebook() {
		display_notes((*nb->get_notes()));
	}
	
	static void display_notes(vector<Note> notes) {
		int i = 1;
		if (notes.size() == 0) {
			cout << "������ �����������" << endl;
			return;
		}
		cout << endl << "---" << endl;
		cout << "�\t" << "creation datetime" << "\t" << "change datetime" << "\t\t" << "author" << "\t\t" << "tags" << endl;
		for_each((notes).begin(), (notes).end(), [&](Note n) {
			cout << i << "\t";
			cout << time_t_to_string(n.get_creation_date()) << "\t";
			cout << time_t_to_string(n.get_change_date()) << "\t";
			cout << n.get_author() << "\t";
			cout << n.get_tags_to_string() << endl;
			i++;
			});
		cout << "---" << endl;
	}

	// ������� ���������� ����� ������. index >= 0
	void display_note(unsigned int index) {
		Note note = (*(*nb).get_notes())[index];
		cout << note.get_note() << endl;
	}

	// �����
	void display_notebook_with_search(string filter_by_author = "", vector<string> filter_by_tags = vector<string>()) {
		const vector<Note>* notes = (*nb).get_notes();
		int i = 1;

		cout << endl << "---" << endl;
		cout << "�\t" << "creation datetime" << "\t" << "change datetime" << "\t\t" << "author" << "\t\t" << "tags" << endl;
		for_each((*notes).begin(), (*notes).end(), [&](Note n) {
			vector<string> tags = n.get_tags();
			bool fit = true; // ������ ��������

			// ����� �� �����
			if (filter_by_tags.size() != 0) {
				for_each(filter_by_tags.begin(), filter_by_tags.end(), [&](string& s) {
					if (find(tags.begin(), tags.end(), s) == tags.end()) // ����� ���� ����� ����� ������
					{
						fit = false;
					}
					});
			}

			// ����� �� ������
			if (filter_by_author != "" && filter_by_author != n.get_author()) {
				fit = false;
			}

			if (fit) {
				// ���� ������ �������� - �������
				cout << i << "\t";
				cout << time_t_to_string(n.get_creation_date()) << "\t";
				cout << time_t_to_string(n.get_change_date()) << "\t";
				cout << n.get_author() << "\t";
				cout << n.get_tags_to_string() << endl;
			}
			i++;
			});
		cout << "---" << endl;
	}

	// ������ � ����
	bool write_notes(string path, string file = "my_notes")
	{
		string new_file = path + "\\" + file + ".notes";
		ofstream f;
		try {
			f.open(new_file);
			const vector<Note>* notes = (*nb).get_notes();

			// ������ � ���� ������ ������ ���������
			for_each((*notes).begin(), (*notes).end(), [&](Note n) {
				f << n.get_creation_date() << delimiter;
				f << n.get_change_date() << delimiter;
				f << n.get_author() << delimiter;
				vector<string> tags = n.get_tags();
				for (auto const& el : tags) f << el + "#";
				f << delimiter;
				f << n.get_note() << endl;
				});

			f.close();
			return true;
		}
		catch (exception ex) {
			return false;
		}
	}

	// ������ �� �����
	bool read_notes(string file)
	{
		string choice;
		cout << "������ ������������ �������� ������ ��� �������� ������ � ������������? (�-������������, ����-��������)";
		cin >> choice;
		if (choice[0] == '�') {
			nb->set_null(); // ��������� ������ ��� ����������
		}

		const vector<Note>* notes = (*nb).get_notes(); // ��������������� ������ ��� ������������ �������

		try {
			ifstream input(file);
			for (string line; getline(input, line); )
			{
				// ��������� �� �����������
				vector<string> elements = split(line, " | ");
				vector<string> tags = split(elements[3], "#"); // ���������� �����
				tags.pop_back();
				// ���������� ����� - ���� ������ (���� ������ ������ �������� �����������)
				string note;
				for_each(elements.begin() + 4, elements.end(), [&](const string& piece) { note += piece; });

				nb->add_note(note, elements[2], stoll(elements[0]), stoll(elements[1]), tags);
			}

			input.close();
			return true;
		}
		catch (exception ex) {
			return false;
		}
	}

	// ���������� ������ ������1#������2#������3# �� ���������
	static vector<string> split(string str, string token) {
		vector<string>result;
		while (str.size()) {
			int index = str.find(token);
			if (index != string::npos) {
				result.push_back(str.substr(0, index));
				str = str.substr(index + token.size());
				if (str.size() == 0) result.push_back(str);
			}
			else {
				result.push_back(str);
				str = "";
			}
		}
		return result;
	}

	// ����������� ���� � ������ ��� ������
	static string time_t_to_string(time_t datetime) {
		char str[100];
		tm ltm;
		localtime_s(&ltm, &datetime);
		strftime(str, 100, "%d/%m/%Y %T", &ltm);
		return str;
	}
};