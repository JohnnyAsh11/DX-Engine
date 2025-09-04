#ifndef __SIMULATION_H_
#define __SIMULATION_H_

/// <summary>
/// Defines the actual simulation creating 
/// what is rendered to the window.
/// </summary>
class Simulation
{
private:


public:
	Simulation() = default;
	~Simulation();

	// There is no copy operator or constructor.
	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;

	void Init();
	void Update(float a_fDeltaTime);
	void Draw(float a_fDeltaTime);
	void OnResize();
};

#endif //__SIMULATION_H_

