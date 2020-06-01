struct se_word {
  unsigned char type;
  unsigned char refs;
  int data;
};

struct se_word se_add(const struct se_word a, const struct se_word b) {
  struct se_word ret = {a.type, 0,  a.data + b.data};
  return ret;
}
