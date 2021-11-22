message Task {
	double time; // represents the (remaining) cpu time this task needs
	double quantum; // how far the quantum has progresses (for RoundRobin)
	int priority; // task priority
	string emitterName; // name of the emitting module
}
