using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace PokemonOU
{
    public partial class Form1 : Form
    {
        List<Pokemon> atk_oulist = new List<Pokemon>(); //Credits for the OU List goes to Smogon at http://www.smogon.com/dex/xy/tags/ou
        List<Pokemon> def_oulist = new List<Pokemon>();
        Random randomizer = new Random();
        double stab;
        double weaknessModifier;
        double critical;
        public Form1()
        {
            try
            {
                FileStream fs = new FileStream(@"oulist.csv", FileMode.Open, FileAccess.Read); //File is in $project/bin/Debug folder
                StreamReader read = new StreamReader(fs);
                string output = read.ReadToEnd();
                string[] row = output.Split('\n');

                read.Close();
                fs.Close();

                for (int i = 0; i < 48; i++) // Used a hard-coded for-loop instead of a foreach because the StreamReader is including an empty cell at the end of the .csv, which I am unable to remove
                {
                    string[] data = row[i].Split(',');
                    Pokemon newEntry = new Pokemon(data[0], int.Parse(data[1]), int.Parse(data[2]), int.Parse(data[3]), int.Parse(data[4]), int.Parse(data[5]), int.Parse(data[6]), int.Parse(data[7]), int.Parse(data[8]));
                    atk_oulist.Add(newEntry);
                    def_oulist.Add(newEntry);
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.ToString());
            }

            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            moveCat.SelectedIndex = 0;
            moveType.SelectedIndex = 0;
            attacker.DataSource = atk_oulist;
            defender.DataSource = def_oulist;
            mod_atk.SelectedIndex = 6;
            mod_spatk.SelectedIndex = 6;
            mod_def.SelectedIndex = 6;
            mod_spdef.SelectedIndex = 6;
        }

        private void calculate_Click(object sender, EventArgs e)
        {
            //Credits for the damage formula goes to Bulbapedia at http://bulbapedia.bulbagarden.net/wiki/Damage

            double attack_mod;
            double spatk_mod;
            double defense_mod;
            double spdef_mod;

            if (mod_atk.Text == "-6")
            {
                attack_mod = 0.25;
            }
            else if (mod_atk.Text == "-5")
            {
                attack_mod = 0.285;
            }
            else if (mod_atk.Text == "-4")
            {
                attack_mod = 0.33;
            }
            else if (mod_atk.Text == "-3")
            {
                attack_mod = 0.4;
            }
            else if (mod_atk.Text == "-2")
            {
                attack_mod = 0.5;
            }
            else if (mod_atk.Text == "-1")
            {
                attack_mod = 0.66;
            }
            else if (mod_atk.Text == "0")
            {
                attack_mod = 1;
            }
            else if (mod_atk.Text == "1")
            {
                attack_mod = 1.5;
            }
            else if (mod_atk.Text == "2")
            {
                attack_mod = 2;
            }
            else if (mod_atk.Text == "3")
            {
                attack_mod = 2.5;
            }
            else if (mod_atk.Text == "4")
            {
                attack_mod = 3;
            }
            else if (mod_atk.Text == "5")
            {
                attack_mod = 3.5;
            }
            else
            {
                attack_mod = 4;
            } //Attack modifier if-tree

            if (mod_spatk.Text == "-6")
            {
                spatk_mod = 0.25;
            }
            else if (mod_spatk.Text == "-5")
            {
                spatk_mod = 0.285;
            }
            else if (mod_spatk.Text == "-4")
            {
                spatk_mod = 0.33;
            }
            else if (mod_spatk.Text == "-3")
            {
                spatk_mod = 0.4;
            }
            else if (mod_spatk.Text == "-2")
            {
                spatk_mod = 0.5;
            }
            else if (mod_spatk.Text == "-1")
            {
                spatk_mod = 0.66;
            }
            else if (mod_spatk.Text == "0")
            {
                spatk_mod = 1;
            }
            else if (mod_spatk.Text == "1")
            {
                spatk_mod = 1.5;
            }
            else if (mod_spatk.Text == "2")
            {
                spatk_mod = 2;
            }
            else if (mod_spatk.Text == "3")
            {
                spatk_mod = 2.5;
            }
            else if (mod_spatk.Text == "4")
            {
                spatk_mod = 3;
            }
            else if (mod_spatk.Text == "5")
            {
                spatk_mod = 3.5;
            }
            else
            {
                spatk_mod = 4;
            } //Special attack modifier if-tree

            if (mod_def.Text == "-6")
            {
                defense_mod = 0.25;
            }
            else if (mod_def.Text == "-5")
            {
                defense_mod = 0.285;
            }
            else if (mod_def.Text == "-4")
            {
                defense_mod = 0.33;
            }
            else if (mod_def.Text == "-3")
            {
                defense_mod = 0.4;
            }
            else if (mod_def.Text == "-2")
            {
                defense_mod = 0.5;
            }
            else if (mod_def.Text == "-1")
            {
                defense_mod = 0.66;
            }
            else if (mod_def.Text == "0")
            {
                defense_mod = 1;
            }
            else if (mod_def.Text == "1")
            {
                defense_mod = 1.5;
            }
            else if (mod_def.Text == "2")
            {
                defense_mod = 2;
            }
            else if (mod_def.Text == "3")
            {
                defense_mod = 2.5;
            }
            else if (mod_def.Text == "4")
            {
                defense_mod = 3;
            }
            else if (mod_def.Text == "5")
            {
                defense_mod = 3.5;
            }
            else
            {
                defense_mod = 4;
            } //Defense modifier if-tree

            if (mod_spdef.Text == "-6")
            {
                spdef_mod = 0.25;
            }
            else if (mod_spdef.Text == "-5")
            {
                spdef_mod = 0.285;
            }
            else if (mod_spdef.Text == "-4")
            {
                spdef_mod = 0.33;
            }
            else if (mod_spdef.Text == "-3")
            {
                spdef_mod = 0.4;
            }
            else if (mod_spdef.Text == "-2")
            {
                spdef_mod = 0.5;
            }
            else if (mod_spdef.Text == "-1")
            {
                spdef_mod = 0.66;
            }
            else if (mod_spdef.Text == "0")
            {
                spdef_mod = 1;
            }
            else if (mod_spdef.Text == "1")
            {
                spdef_mod = 1.5;
            }
            else if (mod_spdef.Text == "2")
            {
                spdef_mod = 2;
            }
            else if (mod_spdef.Text == "3")
            {
                spdef_mod = 2.5;
            }
            else if (mod_spdef.Text == "4")
            {
                spdef_mod = 3;
            }
            else if (mod_spdef.Text == "5")
            {
                spdef_mod = 3.5;
            }
            else
            {
                spdef_mod = 4;
            } //Special defense modifier if-tree

            double atk_def_quotient = ((double)atk_oulist[attacker.SelectedIndex].Attack * attack_mod) / ((double)def_oulist[defender.SelectedIndex].Defense * defense_mod);
            double spatk_spdef_quotient = ((double)atk_oulist[attacker.SelectedIndex].Spatk * spatk_mod)/ ((double)def_oulist[defender.SelectedIndex].Spdef * spdef_mod);
            double level_quotient = 0.84;
            double bp = double.Parse(basePower.Text);
            
            //double modifier = stab * weaknessModifier * critical;

            double rand = randomizer.NextDouble() * (1.00 - 0.85) + 0.85; //randomizes between 0.85 to 1.00 for damage variation
            double rand2 = randomizer.NextDouble();

            if (rand2 < 0.0625)
            {
                critical = 1.5;
            }
            else
            {
                critical = 1;
            } //Critical checker

            if (atk_type1.Text == moveType.SelectedItem.ToString() || atk_type2.Text == moveType.SelectedItem.ToString())
            {
                stab = 1.5;
            }
            else
            {
                stab = 1;
            } //STAB checker

            if (moveCat.SelectedIndex == 0)
            {
                int damage_total = (int)Math.Floor((level_quotient * atk_def_quotient * bp + 2) * stab * rand * critical * weaknessModifier);
                if (rand2 < 0.0625)
                {
                    damage.Text = string.Format("{0}, Critical Hit!", damage_total);
                }
                else
                {
                    damage.Text = string.Format("{0}", damage_total);
                }

                //MessageBox.Show(string.Format("{0}", atk_def_quotient.ToString()));
            }
            else
            {
                int damage_total = (int)Math.Floor((level_quotient * spatk_spdef_quotient * bp + 2) * stab * rand * critical * weaknessModifier);
                if (rand2 < 0.0625)
                {
                    damage.Text = string.Format("{0}, Critical Hit!", damage_total);
                }
                else
                {
                    damage.Text = string.Format("{0}", damage_total);
                }
                //MessageBox.Show(string.Format("{0}", spatk_spdef_quotient.ToString()));
            } //Modifies Damage label, indicates Critical Hits
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int type1 = atk_oulist[attacker.SelectedIndex].Type1;
            int type2 = atk_oulist[attacker.SelectedIndex].Type2;
            atk_stat.Text = string.Format("{0}/{1}/{2}/{3}/{4}/{5}", atk_oulist[attacker.SelectedIndex].Hp, atk_oulist[attacker.SelectedIndex].Attack, atk_oulist[attacker.SelectedIndex].Defense, atk_oulist[attacker.SelectedIndex].Spatk, atk_oulist[attacker.SelectedIndex].Spdef, atk_oulist[attacker.SelectedIndex].Speed);

            if (type1 == 1)
            {
                atk_type1.Text = "Normal";
            }
            else if (type1 == 2)
            {
                atk_type1.Text = "Fighting";
            }
            else if (type1 == 3)
            {
                atk_type1.Text = "Flying";
            }
            else if (type1 == 4)
            {
                atk_type1.Text = "Poison";
            }
            else if (type1 == 5)
            {
                atk_type1.Text = "Ground";
            }
            else if (type1 == 6)
            {
                atk_type1.Text = "Rock";
            }
            else if (type1 == 7)
            {
                atk_type1.Text = "Bug";
            }
            else if (type1 == 8)
            {
                atk_type1.Text = "Ghost";
            }
            else if (type1 == 9)
            {
                atk_type1.Text = "Steel";
            }
            else if (type1 == 10)
            {
                atk_type1.Text = "Fire";
            }
            else if (type1 == 11)
            {
                atk_type1.Text = "Water";
            }
            else if (type1 == 12)
            {
                atk_type1.Text = "Grass";
            }
            else if (type1 == 13)
            {
                atk_type1.Text = "Electric";
            }
            else if (type1 == 14)
            {
                atk_type1.Text = "Psychic";
            }
            else if (type1 == 15)
            {
                atk_type1.Text = "Ice";
            }
            else if (type1 == 16)
            {
                atk_type1.Text = "Dragon";
            }
            else if (type1 == 17)
            {
                atk_type1.Text = "Dark";
            }
            else
            {
                atk_type1.Text = "Fairy";
            } //Type1 if-tree for Attacker

            if (type2 == 1)
            {
                atk_type2.Text = "Normal";
            }
            else if (type2 == 2)
            {
                atk_type2.Text = "Fighting";
            }
            else if (type2 == 3)
            {
                atk_type2.Text = "Flying";
            }
            else if (type2 == 4)
            {
                atk_type2.Text = "Poison";
            }
            else if (type2 == 5)
            {
                atk_type2.Text = "Ground";
            }
            else if (type2 == 6)
            {
                atk_type2.Text = "Rock";
            }
            else if (type2 == 7)
            {
                atk_type2.Text = "Bug";
            }
            else if (type2 == 8)
            {
                atk_type2.Text = "Ghost";
            }
            else if (type2 == 9)
            {
                atk_type2.Text = "Steel";
            }
            else if (type2 == 10)
            {
                atk_type2.Text = "Fire";
            }
            else if (type2 == 11)
            {
                atk_type2.Text = "Water";
            }
            else if (type2 == 12)
            {
                atk_type2.Text = "Grass";
            }
            else if (type2 == 13)
            {
                atk_type2.Text = "Electric";
            }
            else if (type2 == 14)
            {
                atk_type2.Text = "Psychic";
            }
            else if (type2 == 15)
            {
                atk_type2.Text = "Ice";
            }
            else if (type2 == 16)
            {
                atk_type2.Text = "Dragon";
            }
            else if (type2 == 17)
            {
                atk_type2.Text = "Dark";
            }
            else
            {
                atk_type2.Text = "Fairy";
            } // Type2 if-tree for Attacker
        }

        private void defender_SelectedIndexChanged(object sender, EventArgs e)
        {
            int type1 = def_oulist[defender.SelectedIndex].Type1;
            int type2 = def_oulist[defender.SelectedIndex].Type2;
            def_stat.Text = string.Format("{0}/{1}/{2}/{3}/{4}/{5}", def_oulist[defender.SelectedIndex].Hp, def_oulist[defender.SelectedIndex].Attack, def_oulist[defender.SelectedIndex].Defense, def_oulist[defender.SelectedIndex].Spatk, def_oulist[defender.SelectedIndex].Spdef, def_oulist[defender.SelectedIndex].Speed);
            if (type1 == 1)
            {
                def_type1.Text = "Normal";
            }
            else if (type1 == 2)
            {
                def_type1.Text = "Fighting";
            }
            else if (type1 == 3)
            {
                def_type1.Text = "Flying";
            }
            else if (type1 == 4)
            {
                def_type1.Text = "Poison";
            }
            else if (type1 == 5)
            {
                def_type1.Text = "Ground";
            }
            else if (type1 == 6)
            {
                def_type1.Text = "Rock";
            }
            else if (type1 == 7)
            {
                def_type1.Text = "Bug";
            }
            else if (type1 == 8)
            {
                def_type1.Text = "Ghost";
            }
            else if (type1 == 9)
            {
                def_type1.Text = "Steel";
            }
            else if (type1 == 10)
            {
                def_type1.Text = "Fire";
            }
            else if (type1 == 11)
            {
                def_type1.Text = "Water";
            }
            else if (type1 == 12)
            {
                def_type1.Text = "Grass";
            }
            else if (type1 == 13)
            {
                def_type1.Text = "Electric";
            }
            else if (type1 == 14)
            {
                def_type1.Text = "Psychic";
            }
            else if (type1 == 15)
            {
                def_type1.Text = "Ice";
            }
            else if (type1 == 16)
            {
                def_type1.Text = "Dragon";
            }
            else if (type1 == 17)
            {
                def_type1.Text = "Dark";
            }
            else
            {
                def_type1.Text = "Fairy";
            } // Type1 if-tree for Defender

            if (type2 == 1)
            {
                def_type2.Text = "Normal";
            }
            else if (type2 == 2)
            {
                def_type2.Text = "Fighting";
            }
            else if (type2 == 3)
            {
                def_type2.Text = "Flying";
            }
            else if (type2 == 4)
            {
                def_type2.Text = "Poison";
            }
            else if (type2 == 5)
            {
                def_type2.Text = "Ground";
            }
            else if (type2 == 6)
            {
                def_type2.Text = "Rock";
            }
            else if (type2 == 7)
            {
                def_type2.Text = "Bug";
            }
            else if (type2 == 8)
            {
                def_type2.Text = "Ghost";
            }
            else if (type2 == 9)
            {
                def_type2.Text = "Steel";
            }
            else if (type2 == 10)
            {
                def_type2.Text = "Fire";
            }
            else if (type2 == 11)
            {
                def_type2.Text = "Water";
            }
            else if (type2 == 12)
            {
                def_type2.Text = "Grass";
            }
            else if (type2 == 13)
            {
                def_type2.Text = "Electric";
            }
            else if (type2 == 14)
            {
                def_type2.Text = "Psychic";
            }
            else if (type2 == 15)
            {
                def_type2.Text = "Ice";
            }
            else if (type2 == 16)
            {
                def_type2.Text = "Dragon";
            }
            else if (type2 == 17)
            {
                def_type2.Text = "Dark";
            }
            else
            {
                def_type2.Text = "Fairy";
            } // Type2 if-tree for Defender
        }

        private void moveType_SelectedIndexChanged(object sender, EventArgs e)
        {
            //TYPE LEGEND:
            //normal = 1
            //fighting = 2
            //flying = 3
            //poison = 4
            //ground = 5
            //rock = 6
            //bug = 7
            //ghost = 8
            //steel = 9
            //fire = 10
            //water = 11
            //grass = 12
            //electric = 13
            //psychic = 14
            //ice = 15
            //dragon = 16
            //dark = 17
            //fairy = 18

            if (moveType.SelectedItem.ToString() == "Normal")
            {
                weaknessModifier = 1;
            }
            else if (moveType.SelectedItem.ToString() == "Fighting")
            {
                if (def_type1.Text == "Normal")
                {
                    weaknessModifier = 2;
                }
            }
            else if (moveType.SelectedItem.ToString() == "Flying")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Poison")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Ground")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Rock")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Bug")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Ghost")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Steel")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Fire")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Water")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Grass")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Electric")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Psychic")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Ice")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Dragon")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Dark")
            {
            }
            else if (moveType.SelectedItem.ToString() == "Fairy")
            {
            } //Weakness if-tree
        }
    }
}
