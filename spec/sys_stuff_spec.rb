# frozen_string_literal: true
RSpec.describe SysStuff do
  it "has a version number" do
    expect(SysStuff::VERSION).not_to be nil
  end
end