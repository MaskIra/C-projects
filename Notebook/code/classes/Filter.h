#pragma once
#include "Notebook.h"
#include <vector>
#include <iostream>
#include <algorithm>

class Filter {
protected:
	Notebook* nb;
	vector<Note> filter_notes; // �������� ������
	vector<Note> old_notes; // ��������� ������ (�������� ������ ������ ������ nb)

	Filter(Notebook* nb) : nb(nb), filter_notes(vector<Note>()) { old_notes = *(nb->get_notes()); };
	Filter(Notebook* nb, vector<Note> old_notes) : nb(nb), filter_notes(vector<Note>()), old_notes(old_notes) { };
public:
	Filter() : nb(nullptr) { };

	// ���������������� ������
	void update_data() { } // ���������� ����������� ��� ���������� ������, ���������� �� old_notes
	void check_data() {	} // ������� ���������� ������� ����������
	void make_filtration() { } // ��������� ���������� ����������
	void set_search(vector<string> search) { } // ��������� ������� ��� ����������

	// ��������� ���������� ����������
	vector<Note> get_filter_notes() { return filter_notes; }

	// ��������������� �������:
	// ��������, ������� �� ���� "�� ��������� 3 ���"
	static bool last_3_days(time_t datetime) {
		time_t t = time(0);
		double dif = difftime(t, datetime) / (double)(60 * 60 * 24);;

		if (dif <= 3) // 3 ���
			return true;
		return false;
	}
};
