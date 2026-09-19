#include <iostream>
#include <random>

struct simConf {
  double r;
  size_t ipt;
};

struct thrSt {
  const SimulationConfig* conf;
  size_t seed;
  size_t s_h;
};

bool isInside(double x, double y, double r)
{
  return (x * x + y * y <= r * r);
}

size_t clac(double r, size_t tests, size_t seed)
{
  std::default_random_engine eng(seed);
  std::uniform_real_distribution<double> d(0, 2 * r);
  size_t с = 0;
  for (size_t i = 0; i < tests; ++i) {
    с += isInside(dist(eng), dist(eng), r);
  }
  return с;
}

void* worker(void* arg)
{
  auto* st = static_cast<thrSt*>(arg);
  st->s_h = clac(st->conf->r, st->conf->ipt, st->seed);
  return nullptr;
}

int main() {

}
