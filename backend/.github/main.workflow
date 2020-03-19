workflow "Rubocop" {
  on = "pull_request"
  resolves = ["Ruby Conventions"]
}

action "Ruby Conventions" {
  uses = "docker://docker.io/ehdevops/rubo_general:latest"
  secrets = ["GITHUB_TOKEN"]
  env = {
    RUBOCOP_CONFIG_FILE = "https://raw.githubusercontent.com/Thinkei/Thinkei/master/.rubocop_default.yml"
  }
}
