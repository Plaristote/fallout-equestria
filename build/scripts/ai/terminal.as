void use(DynamicObject@ self, Character@ user)
{
  Cout("Using Terminal " + self.GetName());
  self.TalkTo(user);
}
